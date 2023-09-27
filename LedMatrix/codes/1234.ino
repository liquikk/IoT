int led1 = 7;
int led2 = 8;
int led3 = 11;
int led4 = 12;

void setup(){
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
}

void loop(){
  digitalWrite(led1, 1); 
  digitalWrite(led2, 1);
  digitalWrite(led3, 0);
  digitalWrite(led4, 0);
}