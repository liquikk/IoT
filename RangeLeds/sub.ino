#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int ledPin1 = 5; // D1
const int ledPin2 = 4; // D2
const int ledPin3 = 0; // D3
const int ledPin4 = 2; // D4

const char* ssid = "Lipa"; 
const char* password = "lipay2547";  
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600); 
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  setup_wifi();

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setup_wifi() {
  Serial.println();
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) {
      Serial.println("Не удалось подключиться к Wi-Fi. Перезапустите устройство.");
      return;
    }
  }
  Serial.println("Подключено к Wi-Fi: " + String(WiFi.SSID()));
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Подключение к MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Подключено к MQTT");
      // Подписка на тему MQTT
      Serial.println(client.subscribe("esp/sensor"));
    } else {
      Serial.print("Не удалось подключиться, состояние " + String(client.state()));
      Serial.println("...");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Получено сообщение в теме: " + String(topic));
  Serial.print("Переданное сообщение:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  int distance = atoi((char*)payload);
  Serial.println("Расстояние: " + String(distance));
  distanceLeds(distance);
}

void distanceLeds(int distance) {
  if (distance <= 5) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
  } else if (distance <= 10 && distance >= 5) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, LOW);
  } else if (distance <= 15 && distance >= 10) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
  } else if (distance <= 20 && distance >= 15) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
  } else if (distance >= 20) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
  }
}