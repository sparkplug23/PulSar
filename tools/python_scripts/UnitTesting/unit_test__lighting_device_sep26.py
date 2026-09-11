"""
unit_test__lighting_device_sep26.py

PulSar MQTT regression testing - lighting devices.

Tests:
    test_palettes()
    test_effects()
    test_all_palettes_all_effects()

Designed to grow into a wider PulSar device regression-test framework.
"""

import json
import time
import uuid
import threading
from dataclasses import dataclass, asdict
from datetime import datetime
from pathlib import Path

import paho.mqtt.client as mqtt


# ============================================================================
# Configuration
# ============================================================================

MQTT_BROKER = "192.168.3.70"
MQTT_PORT = 1883

DEVICE_NAME = "office__ws2815panel12v"

TOPIC_COMMAND = f"{DEVICE_NAME}/set/python"
TOPIC_ECHO_RESPONSE = f"{DEVICE_NAME}/status/unit_test"
TOPIC_LWT = f"{DEVICE_NAME}/LWT"

RESULTS_DIR = Path(__file__).parent / "unit_test_results"


# ============================================================================
# Test timing
# ============================================================================

PALETTE_DWELL_SECONDS = 2

EFFECT_DWELL_SECONDS = 3

PALETTE_EFFECT_DWELL_SECONDS = 3

PRESET_DWELL_SECONDS = 10

# Extra recovery allowance after the expected dwell.
#
# Example:
#   effect dwell = 10 seconds
#   timeout allowance = 120 seconds
#
# Maximum wait before failure = 130 seconds.
RESPONSE_TIMEOUT_ALLOWANCE_SECONDS = 120


# ============================================================================
# Test ranges
# ============================================================================

PALETTE_COUNT = 179
EFFECT_COUNT = 185


# ============================================================================
# Test result
# ============================================================================

@dataclass
class TestResult:

    test_group: str
    test_name: str
    command: dict

    token: str
    dwell_seconds: int

    passed: bool = False
    failure_reason: str | None = None

    started: str = ""
    finished: str = ""

    response_seconds: float | None = None

    lwt_event: str | None = None
    response_payload: dict | None = None


# ============================================================================
# MQTT Test Session
# ============================================================================

class MQTTTestSession:

    def __init__(self):

        self.client = mqtt.Client(
            mqtt.CallbackAPIVersion.VERSION2,
            client_id=f"PulSar_UnitTest_{uuid.uuid4().hex[:8]}"
        )

        self.connected = threading.Event()

        self.response_event = threading.Event()
        self.lwt_event = threading.Event()

        self.expected_token = None
        self.response_payload = None

        self.current_lwt_state = None
        self.current_lwt_event = None

        self.test_active = False

        self.results: list[TestResult] = []

        self.client.on_connect = self._on_connect
        self.client.on_message = self._on_message
        self.client.on_disconnect = self._on_disconnect


    # ------------------------------------------------------------------------
    # MQTT connection
    # ------------------------------------------------------------------------

    def connect(self):

        print(
            f"Connecting to MQTT broker "
            f"{MQTT_BROKER}:{MQTT_PORT} ..."
        )

        self.client.connect(
            MQTT_BROKER,
            MQTT_PORT,
            keepalive=30
        )

        self.client.loop_start()

        if not self.connected.wait(timeout=10):
            raise RuntimeError("MQTT connection failed")

        print("MQTT connected")


    def disconnect(self):

        self.client.disconnect()
        self.client.loop_stop()


    # ------------------------------------------------------------------------
    # MQTT callbacks
    # ------------------------------------------------------------------------

    def _on_connect(
        self,
        client,
        userdata,
        flags,
        reason_code,
        properties
    ):

        if reason_code != 0:

            print(
                f"MQTT connection failed: "
                f"{reason_code}"
            )

            return

        client.subscribe(TOPIC_ECHO_RESPONSE)
        client.subscribe(TOPIC_LWT)

        self.connected.set()


    def _on_disconnect(
        self,
        client,
        userdata,
        disconnect_flags,
        reason_code,
        properties
    ):

        self.connected.clear()


    def _on_message(
        self,
        client,
        userdata,
        msg
    ):

        payload_text = msg.payload.decode(
            "utf-8",
            errors="replace"
        )


        # --------------------------------------------------------------------
        # LWT
        # --------------------------------------------------------------------

        if msg.topic == TOPIC_LWT:

            state = payload_text.strip()

            self.current_lwt_state = state

            print(
                f"[LWT] {state}"
                + (" [retained]" if msg.retain else "")
            )

            # Retained LWT only establishes the current known state.
            #
            # A fresh Online or Offline event while a test is active means
            # the device changed MQTT lifecycle state during the test and
            # that test must fail immediately.
            if self.test_active and not msg.retain:

                if state.lower() in (
                    "online",
                    "offline"
                ):

                    self.current_lwt_event = state
                    self.lwt_event.set()

            return


        # --------------------------------------------------------------------
        # EchoAlive response
        # --------------------------------------------------------------------

        if msg.topic == TOPIC_ECHO_RESPONSE:

            try:

                payload = json.loads(
                    payload_text
                )

            except json.JSONDecodeError:

                print(
                    f"[RX] Invalid JSON: "
                    f"{payload_text}"
                )

                return


            echo_alive = payload.get(
                "EchoAlive"
            )

            if not isinstance(
                echo_alive,
                dict
            ):

                print(
                    "[RX] Ignoring response "
                    "without EchoAlive object"
                )

                return


            token = echo_alive.get(
                "Token"
            )

            if token != self.expected_token:

                print(
                    f"[RX] Ignoring stale/unexpected "
                    f"EchoAlive token={token}"
                )

                return


            self.response_payload = payload
            self.response_event.set()


    # ------------------------------------------------------------------------
    # Execute one test command
    # ------------------------------------------------------------------------

    def run_test(
        self,
        test_group: str,
        test_name: str,
        command: dict,
        dwell_seconds: int,
    ) -> TestResult:

        token = uuid.uuid4().hex

        command = dict(
            command
        )

        # The ESP does not respond immediately.
        #
        # It starts a device-side countdown and only publishes the
        # EchoAlive response after DelaySeconds.
        #
        # Receiving the matching token therefore proves that the device
        # remained alive while the requested command was active.
        command["EchoAlive"] = {
            "Token": token,
            "DelaySeconds": int(
                dwell_seconds
            )
        }


        result = TestResult(
            test_group=test_group,
            test_name=test_name,
            command=command,
            token=token,
            dwell_seconds=dwell_seconds,
            started=datetime.now().isoformat(
                timespec="seconds"
            ),
        )


        self.expected_token = token
        self.response_payload = None
        self.current_lwt_event = None

        self.response_event.clear()
        self.lwt_event.clear()

        self.test_active = True


        payload = json.dumps(
            command,
            separators=(",", ":")
        )


        print()
        print("=" * 90)

        print(
            f"{test_group}: "
            f"{test_name} "
            f"[dwell={dwell_seconds}s]"
        )

        print(
            payload
        )


        start_monotonic = (
            time.monotonic()
        )


        publish_result = self.client.publish(
            TOPIC_COMMAND,
            payload,
            qos=0,
            retain=False
        )

        if publish_result.rc != mqtt.MQTT_ERR_SUCCESS:

            result.passed = False

            result.failure_reason = (
                f"mqtt_publish_error_"
                f"{publish_result.rc}"
            )

            result.response_seconds = 0.0

            result.finished = (
                datetime.now().isoformat(
                    timespec="seconds"
                )
            )

            self.test_active = False

            self.results.append(
                result
            )

            print(
                f"[FAIL] MQTT publish failed "
                f"rc={publish_result.rc}"
            )

            return result


        timeout_seconds = (
            dwell_seconds
            + RESPONSE_TIMEOUT_ALLOWANCE_SECONDS
        )


        # --------------------------------------------------------------------
        # Wait for:
        #
        # PASS:
        #   delayed matching EchoAlive
        #
        # FAIL:
        #   fresh Online LWT
        #   fresh Offline LWT
        #   dwell + recovery timeout
        #
        # --------------------------------------------------------------------

        while True:

            elapsed = (
                time.monotonic()
                - start_monotonic
            )


            # ---------------------------------------------------------------
            # Device lifecycle changed
            # ---------------------------------------------------------------

            if self.lwt_event.is_set():

                result.passed = False

                result.lwt_event = (
                    self.current_lwt_event
                )

                result.failure_reason = (
                    "lwt_"
                    + str(
                        self.current_lwt_event
                    ).lower()
                )

                result.response_seconds = (
                    elapsed
                )

                print(
                    f"[FAIL] LWT changed to "
                    f"{self.current_lwt_event} "
                    f"after {elapsed:.2f}s"
                )

                break


            # ---------------------------------------------------------------
            # Delayed device response received
            # ---------------------------------------------------------------

            if self.response_event.is_set():

                result.passed = True

                result.response_seconds = (
                    elapsed
                )

                result.response_payload = (
                    self.response_payload
                )

                print(
                    f"[PASS] EchoAlive received "
                    f"after {elapsed:.2f}s "
                    f"(requested dwell "
                    f"{dwell_seconds}s)"
                )

                break


            # ---------------------------------------------------------------
            # Device did not respond
            # ---------------------------------------------------------------

            if elapsed >= timeout_seconds:

                result.passed = False

                result.failure_reason = (
                    "echo_timeout"
                )

                result.response_seconds = (
                    elapsed
                )

                print(
                    f"[FAIL] No EchoAlive after "
                    f"{elapsed:.2f}s "
                    f"(dwell={dwell_seconds}s, "
                    f"allowance="
                    f"{RESPONSE_TIMEOUT_ALLOWANCE_SECONDS}s)"
                )

                break


            time.sleep(
                0.05
            )


        self.test_active = False

        result.finished = (
            datetime.now().isoformat(
                timespec="seconds"
            )
        )

        self.results.append(
            result
        )

        return result


# ============================================================================
# Lighting Tests
# ============================================================================

def test_palettes(
    session: MQTTTestSession
):

    for palette_id in range(
        PALETTE_COUNT
    ):

        session.run_test(
            test_group="palettes",
            test_name=(
                f"palette_{palette_id}"
            ),
            dwell_seconds=(
                PALETTE_DWELL_SECONDS
            ),
            command={
                "ColourPalette": palette_id
            }
        )


def test_effects(
    session: MQTTTestSession
):

    for effect_id in range(
        EFFECT_COUNT
    ):

        session.run_test(
            test_group="effects_defaults",
            test_name=(
                f"effect_{effect_id}"
            ),
            dwell_seconds=(
                EFFECT_DWELL_SECONDS
            ),
            command={
                "Effects": {
                    "Function": effect_id,
                    "LoadDefaults": True
                }
            }
        )


def test_all_palettes_all_effects(
    session: MQTTTestSession
):

    for effect_id in range(
        EFFECT_COUNT
    ):

        for palette_id in range(
            PALETTE_COUNT
        ):

            session.run_test(
                test_group=(
                    "palette_effect_matrix_defaults"
                ),
                test_name=(
                    f"effect_{effect_id}"
                    f"__palette_{palette_id}"
                ),
                dwell_seconds=(
                    PALETTE_EFFECT_DWELL_SECONDS
                ),
                command={
                    "ColourPalette": palette_id,
                    "Effects": {
                        "Function": effect_id,
                        "LoadDefaults": True
                    }
                }
            )


# ============================================================================
# Reporting
# ============================================================================

def print_results_table(
    results: list[TestResult]
):

    print()
    print("=" * 130)
    print("TEST RESULTS")
    print("=" * 130)


    header = (
        f"{'RESULT':<8} "
        f"{'GROUP':<34} "
        f"{'TEST':<42} "
        f"{'DWELL':>8} "
        f"{'TIME':>10} "
        f"{'REASON'}"
    )

    print(
        header
    )

    print(
        "-" * 130
    )


    for result in results:

        status = (
            "PASS"
            if result.passed
            else "FAIL"
        )


        response = (
            f"{result.response_seconds:.2f}s"
            if result.response_seconds is not None
            else "-"
        )


        reason = (
            result.failure_reason
            or ""
        )


        print(
            f"{status:<8} "
            f"{result.test_group:<34} "
            f"{result.test_name:<42} "
            f"{result.dwell_seconds:>7}s "
            f"{response:>10} "
            f"{reason}"
        )


    passed = sum(
        result.passed
        for result in results
    )


    failed = (
        len(results)
        - passed
    )


    print(
        "-" * 130
    )


    print(
        f"TOTAL={len(results)}   "
        f"PASS={passed}   "
        f"FAIL={failed}"
    )


# ============================================================================
# JSON report
# ============================================================================

def save_results_json(
    results: list[TestResult]
):

    RESULTS_DIR.mkdir(
        parents=True,
        exist_ok=True
    )


    timestamp = (
        datetime.now().strftime(
            "%Y%m%d_%H%M%S"
        )
    )


    filename = (
        RESULTS_DIR
        / (
            f"lighting_"
            f"{DEVICE_NAME}_"
            f"{timestamp}.json"
        )
    )


    output = {

        "device": DEVICE_NAME,

        "timestamp": (
            datetime.now().isoformat(
                timespec="seconds"
            )
        ),

        "mqtt": {
            "broker": MQTT_BROKER,
            "port": MQTT_PORT,
            "command_topic": TOPIC_COMMAND,
            "echo_topic": TOPIC_ECHO_RESPONSE,
            "lwt_topic": TOPIC_LWT,
        },

        "configuration": {

            "palette_count": (
                PALETTE_COUNT
            ),

            "effect_count": (
                EFFECT_COUNT
            ),

            "palette_dwell_seconds": (
                PALETTE_DWELL_SECONDS
            ),

            "effect_dwell_seconds": (
                EFFECT_DWELL_SECONDS
            ),

            "palette_effect_dwell_seconds": (
                PALETTE_EFFECT_DWELL_SECONDS
            ),

            "preset_dwell_seconds": (
                PRESET_DWELL_SECONDS
            ),

            "response_timeout_allowance_seconds": (
                RESPONSE_TIMEOUT_ALLOWANCE_SECONDS
            ),
        },

        "summary": {

            "total": len(
                results
            ),

            "passed": sum(
                result.passed
                for result in results
            ),

            "failed": sum(
                not result.passed
                for result in results
            ),
        },

        "results": [
            asdict(
                result
            )
            for result in results
        ]
    }


    with filename.open(
        "w",
        encoding="utf-8"
    ) as file:

        json.dump(
            output,
            file,
            indent=2
        )


    print()
    print(
        f"JSON report: "
        f"{filename}"
    )


# ============================================================================
# Main
# ============================================================================

def main():

    session = (
        MQTTTestSession()
    )


    try:

        session.connect()


        # --------------------------------------------------------------------
        # Enable whichever test groups are wanted
        # --------------------------------------------------------------------

        # test_palettes(session)

        # test_effects(session)

        test_all_palettes_all_effects(session)


    finally:

        session.disconnect()

        print_results_table(
            session.results
        )

        save_results_json(
            session.results
        )


if __name__ == "__main__":

    main()