import time
import paho.mqtt.client as paho
import datetime
from questionary import confirm

broker = "broker.emqx.io"
client = paho.Client("client-isu-111")
print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Publishing")
min_duration = 20 
max_duration = 40 
current_duration = max_duration  

try:
    while True:
        now = datetime.datetime.now()
        if now.second == 0:
            max_duration -= 1

        # Проверяем и устанавливаем минимальное время свечения
        if max_duration == 30:
            max_duration += 10

        if now.second >= min_duration and now.second <= max_duration:
            state = "0"
        else:
            state = "1"

        current_time = now.strftime("%S")
        print(f'Секунды: {current_time}')
        client.publish("esp8266daiki/command", state)
        time.sleep(1)
        
except KeyboardInterrupt:
    print("\nExiting...")
    client.disconnect()
    client.loop_stop()
