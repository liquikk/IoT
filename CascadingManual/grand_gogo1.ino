#include <SoftwareSerial.h>

SoftwareSerial slave_serial(10, 11);

String neighbours_string = "";
int numberOfNeighbors = 0;
int indexOfNumber = -1;
bool isSynchronized = false;
bool isAmbiguousNumber = false;
bool isOff = false;
String globalNumber = "";
String undeletedGlobalNumber = "";
bool digits[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1, 0},
  {1, 1, 1, 1, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 0},
  {1, 0, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 0}
};
void setup() {
  slave_serial.begin(2400);
  Serial.begin(2400);
  slave_serial.write('d');
  for (int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);  
  }
}
void loop() {
  delay(100);
  char digit = Serial.read();
  
  if (isAmbiguousNumber) {
    delay(2000);
    if (numberOfNeighbors == 2 && !isSynchronized) {
      delay(300);
      isSynchronized = !isSynchronized;
    } else if (numberOfNeighbors == 1 && !isSynchronized) {
      delay(0);
      isSynchronized = !isSynchronized;
    } else if (numberOfNeighbors == 0 && !isSynchronized) {
      isSynchronized = !isSynchronized;
    }
    if (indexOfNumber <= undeletedGlobalNumber.length()) {
      indexOfNumber += 1;
    } else {
          	indexOfNumber = 0; 
    }
    if (undeletedGlobalNumber[indexOfNumber] == 'o') {
            turn_off();
          } else {
          	show_number((int)undeletedGlobalNumber[indexOfNumber]  - '0');
          }
  }
    char rc = slave_serial.read();
    
    int check = (int)(rc - '0');
    if (check < 0) {
      if (neighbours_string.length() > 0) {
      	for (int i = 0; i < neighbours_string.length(); i++) {
          Serial.print(neighbours_string[i]); 
        }
      Serial.print('c');
        Serial.println(" ");
        numberOfNeighbors = neighbours_string.length();
        Serial.println("Total Neighbours:");
        Serial.println(numberOfNeighbors);
      neighbours_string = ""; 
      }
      } else if (rc == 'c') {
      neighbours_string += 'c';
    }
  if (digit == 'd') {
    Serial.println("Waiting...");
    slave_serial.write("d");
     delay(100);
    if (slave_serial.available() > 0) {
      
      Serial.println("Sent request");
    } else {
      Serial.println("Callback");
     Serial.print('c'); 
    }
  } 
  else {
    int number = (int)(digit - '0');
    if (number < 0) { 
      	if (globalNumber.length() > numberOfNeighbors + 1 && globalNumber.length() != 0) {
      		Serial.println("the running line is running");
          undeletedGlobalNumber = "";
          isAmbiguousNumber = true;
          for (int i = 0; i < globalNumber.length(); i++) {
            slave_serial.write(globalNumber[i]); 
          }
          globalNumber += "ooo";
          undeletedGlobalNumber = globalNumber;
          if (numberOfNeighbors == 2) {
          	indexOfNumber = 0;
            delay(100);
          } else if (numberOfNeighbors == 1) {
           	indexOfNumber = 1; 
            delay(0);
          } else if (numberOfNeighbors == 0) {
           	indexOfNumber = 2; 
          }
          
          if (globalNumber[indexOfNumber] == 'o') {
            turn_off();
          } else {
          	show_number((int)globalNumber[indexOfNumber]  - '0');
          }
    	} else if (globalNumber.length() == numberOfNeighbors + 1 && globalNumber.length() != 0) {
          for (int i = 1; i < globalNumber.length(); i++) {
            slave_serial.write(globalNumber[i]); 
          }
          Serial.println("Data sent");
        Serial.println((int)globalNumber[0] - '0');
        show_number((int)globalNumber[0]  - '0');
      } else if (globalNumber.length() <= numberOfNeighbors && globalNumber.length() != 0) {
          for (int i = 0; i < globalNumber.length(); i++) {
            slave_serial.write(globalNumber[i]); 
          }
          Serial.println("Second data sent");
        show_number(0);
      }
      globalNumber = "";
    } else {
      isAmbiguousNumber = false;
      globalNumber += (String) number;
      Serial.println(globalNumber);
    }
  }
}
void turn_off() {
  for (int i = 2; i < 10; i++) {
    digitalWrite(i, HIGH);
  }
}
void show_number(int num) {
  if (num >= 0 && num <= 9) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(i + 2, !digits[num][i]);
    }
  }
}

