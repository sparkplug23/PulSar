import json
import time
import paho.mqtt.client as mqtt  # pip install paho-mqtt

def run_test_palette_cycle(
    broker="localhost",
    port=1883,
    device_name="mydevice",
    response_topic_enabled=True
):
    topic_out = f"{device_name}/set/testscript"
    topic_in = f"{device_name}/status/debug/unit_test_response"

    max_palette_index = 10      # HARD-CODED: Number of palette indices to cycle through
    delay_seconds = 1           # HARD-CODED: Delay between each command in seconds

    def on_message(client, userdata, msg):
        print(f"[Response] {msg.topic}: {msg.payload.decode()}")
Integrated mmWave
About olo (sweeden doing the same? Make sure why mine is better)
Have we approached commercial AP companies?
Syed Zaidi going to email something.
About new Unified model for channels. 
I should look into a LiFi sims, and do some initial simulations to show how it can be used together. We should be able to get heatmaps, maybe “directional” using different optics? How it can be LOS, combined with our RF, and switching.
ETSI - ETSI to transform the standards development landscape with Software Development Groups

60ghz sensing? Should do some sensing simulations.
30th October, DSIT advanced activity day.

    # client = mqtt.Client()
    # if response_topic_enabled:
    #     client.on_message = on_message

    # client.connect(broker, port, 60)
    # client.loop_start()

    # # Notify device that this is a unit test
    # client.publish(f"{device_name}/UNITTEST", "PaletteCycle")

    # # Subscribe to unit test response topic
    # if response_topic_enabled:
    #     client.subscribe(topic_in)

    # # Send palette index commands
    # for i in range(max_palette_index):
    #     payload = {
    #         "UnitTest": {
    #             "Name": "PaletteCycle",
    #             "ResponseRequired": 1
    #         },
    #         "ColourPalette": i
    #     }
    #     payload_json = json.dumps(payload)
    #     print(f"Publishing to {topic_out}: {payload_json}")
    #     client.publish(topic_out, payload_json)
    #     time.sleep(delay_seconds)

    # client.loop_stop()
    # client.disconnect()


if __name__ == "__main__":
    # Example usage
    run_test_palette_cycle(
        broker="localhost",
        port=1883,
        device_name="cm_light_backleft",
        response_topic_enabled=True
    )
