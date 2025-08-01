import time
import paho.mqtt.client as mqtt

PALETTES = [0, 5, 10]      # Discrete/CRGB examples
EFFECTS  = [0, 3, 12, 20]  # Representative subset

def run_test(broker, port, topic_prefix):
    client = mqtt.Client()
    client.connect(broker, port, 60)
    client.loop_start()

    client.publish(f"{topic_prefix}UNITTEST", "EffectPaletteMatrix")

    for palette in PALETTES:
        client.publish(f"{topic_prefix}Palette", str(palette))
        for effect in EFFECTS:
            client.publish(f"{topic_prefix}Effect", str(effect))
            print(f"Testing palette {palette}, effect {effect}")
            time.sleep(2)

    client.loop_stop()
    client.disconnect()

# TODO: Capture performance feedback via MQTT for plotting later
