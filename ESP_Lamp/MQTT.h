#include <PubSubClient.h>

PubSubClient mqtt_client(wifi_client);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("message received on topic");
  Serial.println(topic);
  Serial.print("message is: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void init_MQTT() {
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(callback);
  while (!mqtt_client.connected()) {
    Serial.print("Trying to connect: ");
    Serial.println(mqtt_broker);
    String client_id = "esp8266_" + id();
    bool success = mqtt_client.connect(client_id.c_str());
    if (success) {
      Serial.println("Successfuly connected" + client_id);
    } else {
      Serial.println("Failed to connectd" + client_id);
      Serial.println(mqtt_client.state());
      delay(2000);
    }
  }
}
