#include <SPI.h>

const int latchPin = 5;
const int clockPin = 3;
const int dataPin = 7;

const byte digits[10] = {
  0b11011101, // 0
  0b01010000, // 1
  0b11001110, // 2
  0b11011010, // 3
  0b01010011, // 4
  0b10011011, // 5
  0b10111111, // 6
  0b11010000, // 7
  0b11011111, // 8
  0b11111011  // 9
};

String inputString = "";

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char userInput = Serial.read();
    inputString += userInput;

    if (inputString.length() == 4) {
      int timeValue = inputString.toInt();

      int minutes = timeValue / 100;
      int seconds = timeValue % 100;

      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 6; j++) {
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin, clockPin, LSBFIRST, digits[seconds % 10]);
          shiftOut(dataPin, clockPin, LSBFIRST, digits[seconds / 10]);
          shiftOut(dataPin, clockPin, LSBFIRST, digits[minutes % 10]);
          shiftOut(dataPin, clockPin, LSBFIRST, digits[minutes / 10]);
          digitalWrite(latchPin, HIGH);

          seconds++;
          if (seconds == 60) {
            minutes++;
            seconds = 0;
          }

          delay(1000);
        }
      }

      inputString = "";
    }
  }
}