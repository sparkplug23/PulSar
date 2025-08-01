import importlib
from config import MQTT_BROKER, MQTT_PORT, MQTT_KEEPALIVE

MQTT_TOPIC_PREFIX = "cm_office__pebble_playlists"

# Define test cases to run (uncomment to include)
TestCase_List = [
    "test_palette_cycle",
    # "test_all_effects",
    # "test_effect_palette_matrix"
]

if __name__ == '__main__':
    for test_case in TestCase_List:
        module = importlib.import_module(f"UnitTests.{test_case}")
        print(f"Running test: {test_case}")

        # Convert "test_palette_cycle" -> "run_test_palette_cycle"
        function_name = f"run_{test_case}"
        run_function = getattr(module, function_name)
        run_function(
            broker=MQTT_BROKER,
            port=MQTT_PORT,
            device_name=MQTT_TOPIC_PREFIX
        )
