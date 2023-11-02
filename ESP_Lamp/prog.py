import time
import paho.mqtt.client as paho
from questionary import prompt, text

mqtt_broker = "broker.emqx.io"
mqtt_port = 1883

# ID лампочки (топик)
lamp_topic = "esp8266daiki/command"

# Время работы лампочки (в сек)
lamp_run_time = 20
min_lamp_run_time = 10
time_decrement = 1

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(lamp_topic)

def on_message(client, userdata, msg):
    print(f"Received message: {msg.payload}")
    
def control_lamp(client, command):
    client.publish(lamp_topic, command)

client= paho.Client("isu100123") 
client.on_connect = on_connect
client.on_message = on_message
client.connect(mqtt_broker, mqtt_port, 60)
client.loop_start()

# Вкл лампочки
control_lamp(client, "on")
current_run_time = lamp_run_time

try:
    while True:
        # время работы лампочки
        control_lamp(client, "on")
        time.sleep(current_run_time)
        
        # Уменьшение время работы лампочки на 1 секунду
        current_run_time = max(current_run_time - time_decrement, min_lamp_run_time)
        
        # Выключение лампочки
        control_lamp(client, "off")
        
        # Сбрасываем время работы лампочки до нач значения
        if current_run_time == min_lamp_run_time:
            current_run_time = lamp_run_time

except KeyboardInterrupt:
    print("\nExiting...")
    client.loop_stop()
    client.disconnect()