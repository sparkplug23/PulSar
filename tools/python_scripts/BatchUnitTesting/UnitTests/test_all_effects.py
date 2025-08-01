import time
import paho.mqtt.client as mqtt

def run_test(broker, port, topic_prefix):
    client = mqtt.Client()
    client.connect(broker, port, 60)
    client.loop_start()

    client.publish(f"{topic_prefix}UNITTEST", "EffectCycle")

    for effect_id in range(64):  # Adjust range if needed
        topic = f"{topic_prefix}Effect"
        print(f"Set effect to {effect_id}")
        client.publish(topic, str(effect_id))
        time.sleep(2)

    client.loop_stop()
    client.disconnect()
