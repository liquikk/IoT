#define sensor_pin A0
#define led_pin 13

bool streaming = false;
bool send_one_value = false;
long previous_send_time = 0;
long send_count = 0;
bool user_command_override = false;
bool auto_mode = true;
bool alarm_mode = false;
int light_threshold = 500;

void setup()
{
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
}

void loop()
{
  long current_time = millis();
  data_reading();

  if (streaming && !user_command_override && current_time / 100 != send_count) {
    send_photo_data();
    previous_send_time += 100;
    send_count = current_time / 100;
    Serial.print("Time in millis:");
    Serial.println(millis());
  }

  if (send_one_value && !user_command_override) {
    send_photo_data();
    send_one_value = false;
  }

  if (auto_mode && !user_command_override && !alarm_mode) {
    int val = analogRead(sensor_pin);
    if (val < light_threshold) {
      digitalWrite(led_pin, HIGH);
    } else {
      digitalWrite(led_pin, LOW);
    }
  }

  if (alarm_mode) {
    blink_led();
  }
}

void send_photo_data() {
  int val = analogRead(sensor_pin);
  Serial.print("Sensor value:");
  Serial.println(val);
}

void data_reading() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'p') {
      streaming = true;
      user_command_override = false;
    } else if (command == 's') {
      streaming = false;
      send_one_value = true;
      user_command_override = false;
    } else if (command == 'c') {  // Включение светодиода
      digitalWrite(led_pin, HIGH);
      user_command_override = true;
    } else if (command == 'd') {  // Выключение светодиода
      digitalWrite(led_pin, LOW);
      user_command_override = true;
    } else if (command == 'a') {  // Включить автоматический режим
      auto_mode = true;
      user_command_override = false;
    } else if (command == 'm') {  // Включить ручной режим
      auto_mode = false;
      user_command_override = false;
    } else if (command == 'l') {  // Включить режим сигнализации
      alarm_mode = true;
      user_command_override = true;
    } else if (command == 'n') {  // Выключить режим сигнализации
      alarm_mode = false;
      user_command_override = true;
      digitalWrite(led_pin, LOW);
    } else {
      streaming = false;
      user_command_override = false;
    }
  }
}

void blink_led() {
  static bool led_state = false;
  static unsigned long previous_time = 0;
  unsigned long current_time = millis();

  if (current_time - previous_time >= 500) {
    led_state = !led_state;
    digitalWrite(led_pin, led_state);
    previous_time = current_time;
  }
}