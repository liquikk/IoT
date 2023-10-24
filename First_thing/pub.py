import time 
import paho.mqtt.client as paho
import random

broker = "broker.emqx.io"

client = paho.Client("client-isu-002")

print("connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Publishing")

# for _ in range(10):
#     state = "on" if random.randint(0,1) else "test"  
#     print(f"state is {state}")
#     client.publish("house/bulb1",state)
#     time.sleep(random.randint(4,10))

while True:
    state = input()
    client.publish("esp8266dada/bobo", state)
    time.sleep(2)
    if state == "1":
        break

client.disconnect()
client.loop_stop()