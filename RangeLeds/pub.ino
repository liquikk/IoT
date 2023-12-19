#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int trigPin = 5;  // Передатчик D1
const int echoPin = 4;  // Приемник D2

const char* wifi_ssid = "liquikk";
const char* wifi_password = "2k11pp8s52";
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

void connectToWiFi() {
  Serial.println("Подключение к Wi-Fi..");
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Подключение к Wi-Fi..");
  }
  Serial.print("Подключено к Wi-Fi: ");
  Serial.println(WiFi.SSID());
}

void connectToMQTT() {
  Serial.println("Подключение к MQTT...");
  while (!client.connected()) {
    if (client.connect("ESP8266_me")) {
      Serial.println("Подключено к MQTT");
    } else {
      Serial.print("Не удалось подключиться, состояние ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void measureDistanceAndPublish() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  float distanceCm = duration * 0.0343 / 2.0;

  if (client.publish("esp/sensor", String(distanceCm).c_str())) {
    Serial.println("Сообщение успешно отправлено: " + String(distanceCm));
  } else {
    Serial.println("Не удалось отправить сообщение");
  }
}

void handleMQTTMessages() {
  client.loop();
}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  connectToMQTT();
}

void loop() {
  measureDistanceAndPublish();
  handleMQTTMessages();
  delay(2000);
}