#define LT_MODULE_L A2 
#define LT_MODULE_F A1 
#define LT_MODULE_R A0 
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define ENA 6
#define ENB 5 
#define IN1 7 
#define IN2 3 
#define IN3 4 
#define IN4 2 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
Servo myservo;
int val;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int speed = 150;
int g_carDirection = 0;
int location = 0;
int ming[9] = {0,0,0,0,0,0,0,0,0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); //ENA
  pinMode(IN1, OUTPUT); //IN1
  pinMode(IN2, OUTPUT); //IN2
  pinMode(ENB, OUTPUT); //ENB
  pinMode(IN3, OUTPUT); //IN3
  pinMode(IN4, OUTPUT); //IN4
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  lcd.init();
  lcd.backlight();
  myservo.attach(10);
}

void init_line_tracer_modules() {
  // put your setup code here, to run once:
  pinMode(LT_MODULE_L, INPUT);
  pinMode(LT_MODULE_F, INPUT);
  pinMode(LT_MODULE_R, INPUT);
}

bool LEFT(){
  int ret = analogRead(LT_MODULE_L);
  return (ret > 550) ? (true) : (false);
}

bool FORWARD(){
  int ret = analogRead(LT_MODULE_F);
  return (ret > 550) ? (true) : (false);
}

bool RIGHT(){
  int ret = analogRead(LT_MODULE_R);
  return (ret > 550) ? (true) : (false);
}

void detect1(){
  int cm;
  int dong = 0;
  val = 120;
  myservo.write(val);
  while(val > 65){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    cm = pulseIn(ECHO_PIN, HIGH)/29/2;
    if(cm < 40){
      dong ++;
    }
    val = val - 5;
    myservo.write(val);
    delay(50);
  }
  if(dong > 4){
    ming[2*location-2] = 1;
  }
  delay(200);
  val = 0;
  dong = 0;
  myservo.write(val);
  while(val < 19){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    cm = pulseIn(ECHO_PIN, HIGH)/29/2;
    if(cm < 40){
      dong ++;
    }
    val = val + 1;
    myservo.write(val);
    delay(50);
  }
  if(dong > 4){
    ming[2*location-1] = 1;
  }
}

void detect2(){
  int cm;
  int dong = 0;
  val = 120;
  myservo.write(val);
  while(val > 64){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    cm = pulseIn(ECHO_PIN, HIGH)/29/2;
    if(cm < 40){
      dong ++;
    }
    val = val - 5;
    myservo.write(val);
    delay(50);
  }
  if(dong > 4){
    ming[8] = 1;
  }
}

void change(int A[], int a, int b){
  int temp;
  temp = A[a];
  A[a] = A[b];
  A[b] = temp;
}

void printfunction(){
  change(ming, 6, 8);
  change(ming, 1, 3);
  change(ming, 1, 4);
  change(ming, 1, 2);
  lcd.setCursor(0,0);
  for(int i = 0 ; i < 9; i++){
    lcd.print(ming[i]);
  }
  while(1){
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(LEFT() && FORWARD() & RIGHT()){
    location++;
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    if(location == 5){
      detect2();
      printfunction();
    }
    else if(location == 3){
      detect1();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
      delay(300);
      while(!(FORWARD() && !(LEFT()) && !(RIGHT()))){
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, speed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, speed);
      }
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      delay(200);
    }
    else if(location == 4){
      detect1();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
      delay(500);
      while(!(FORWARD() && !(LEFT()) && !(RIGHT()))){
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, speed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, speed);
      }
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      delay(200);
    }
    else{
      detect1();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      delay(500);
    }
  }
  else if(FORWARD() && !(LEFT()) && !(RIGHT())){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
  }
  else if(!(FORWARD()) && !(LEFT()) && !(RIGHT())){
    while(!FORWARD() && !LEFT()){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
    }
  }
  else if(!(FORWARD()) && LEFT() && !(RIGHT())){
    while(!(FORWARD())){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
    }
  }
  else if(FORWARD() && LEFT() && !(RIGHT())){
    while(LEFT()){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      if(LEFT() && FORWARD() & RIGHT()){
        return;
      }
    }
  }
  else if(RIGHT() && !(LEFT()) && !(FORWARD())){
    while(RIGHT()){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
    }
  }
  else if(RIGHT() && !(LEFT()) && FORWARD()){
    while(RIGHT()){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
      if(LEFT() && FORWARD() & RIGHT()){
        return;
      }
    }
  }
  Serial.println(location);

}
#define LT_MODULE_L A2 
#define LT_MODULE_F A1 
#define LT_MODULE_R A0 
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define ENA 6
#define ENB 5 
#define IN1 7 
#define IN2 3 
#define IN3 4 
#define IN4 2 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>
Servo myservo;
int val;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int speed = 150;
int g_carDirection = 0;
int location = 0;
int ming[9] = {0,0,0,0,0,0,0,0,0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); //ENA
  pinMode(IN1, OUTPUT); //IN1
  pinMode(IN2, OUTPUT); //IN2
  pinMode(ENB, OUTPUT); //ENB
  pinMode(IN3, OUTPUT); //IN3
  pinMode(IN4, OUTPUT); //IN4
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  lcd.init();
  lcd.backlight();
  myservo.attach(10);
}

void init_line_tracer_modules() {
  // put your setup code here, to run once:
  pinMode(LT_MODULE_L, INPUT);
  pinMode(LT_MODULE_F, INPUT);
  pinMode(LT_MODULE_R, INPUT);
}

bool LEFT(){
  int ret = analogRead(LT_MODULE_L);
  return (ret > 550) ? (true) : (false);
}

bool FORWARD(){
  int ret = analogRead(LT_MODULE_F);
  return (ret > 550) ? (true) : (false);
}

bool RIGHT(){
  int ret = analogRead(LT_MODULE_R);
  return (ret > 550) ? (true) : (false);
}

void detect1(){
  int cm;
  int dong = 0;
  val = 120;
  myservo.write(val);
  while(val > 65){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    cm = pulseIn(ECHO_PIN, HIGH)/29/2;
    if(cm < 40){
      dong ++;
    }
    val = val - 5;
    myservo.write(val);
    delay(50);
  }
  if(dong > 4){
    ming[2*location-2] = 1;
  }
  delay(200);
  val = 0;
  dong = 0;
  myservo.write(val);
  while(val < 19){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    cm = pulseIn(ECHO_PIN, HIGH)/29/2;
    if(cm < 40){
      dong ++;
    }
    val = val + 1;
    myservo.write(val);
    delay(50);
  }
  if(dong > 4){
    ming[2*location-1] = 1;
  }
}

void detect2(){
  int cm;
  int dong = 0;
  val = 120;
  myservo.write(val);
  while(val > 64){
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    cm = pulseIn(ECHO_PIN, HIGH)/29/2;
    if(cm < 40){
      dong ++;
    }
    val = val - 5;
    myservo.write(val);
    delay(50);
  }
  if(dong > 4){
    ming[8] = 1;
  }
}

void change(int A[], int a, int b){
  int temp;
  temp = A[a];
  A[a] = A[b];
  A[b] = temp;
}

void printfunction(){
  change(ming, 6, 8);
  change(ming, 1, 3);
  change(ming, 1, 4);
  change(ming, 1, 2);
  lcd.setCursor(0,0);
  for(int i = 0 ; i < 9; i++){
    lcd.print(ming[i]);
  }
  while(1){
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(LEFT() && FORWARD() & RIGHT()){
    location++;
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    if(location == 5){
      detect2();
      printfunction();
    }
    else if(location == 3){
      detect1();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
      delay(300);
      while(!(FORWARD() && !(LEFT()) && !(RIGHT()))){
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, speed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, speed);
      }
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      delay(200);
    }
    else if(location == 4){
      detect1();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
      delay(500);
      while(!(FORWARD() && !(LEFT()) && !(RIGHT()))){
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, speed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, speed);
      }
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      delay(200);
    }
    else{
      detect1();
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      delay(500);
    }
  }
  else if(FORWARD() && !(LEFT()) && !(RIGHT())){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
  }
  else if(!(FORWARD()) && !(LEFT()) && !(RIGHT())){
    while(!FORWARD() && !LEFT()){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
    }
  }
  else if(!(FORWARD()) && LEFT() && !(RIGHT())){
    while(!(FORWARD())){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
    }
  }
  else if(FORWARD() && LEFT() && !(RIGHT())){
    while(LEFT()){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      if(LEFT() && FORWARD() & RIGHT()){
        return;
      }
    }
  }
  else if(RIGHT() && !(LEFT()) && !(FORWARD())){
    while(RIGHT()){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
    }
  }
  else if(RIGHT() && !(LEFT()) && FORWARD()){
    while(RIGHT()){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
      if(LEFT() && FORWARD() & RIGHT()){
        return;
      }
    }
  }
  Serial.println(location);

}

