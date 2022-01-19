#define LT_MODULE_L A2 
#define LT_MODULE_F A1 
#define LT_MODULE_R A0 
#define ENA 6
#define ENB 5 
#define IN1 7 
#define IN2 3 
#define IN3 4 
#define IN4 2 

int speed = 150;
int g_carDirection = 0;
int CAR_DIR_ST = 1;
int CAR_DIR_FW = 2;
int CAR_DIR_RF = 3;
int CAR_DIR_LF = 4;
int CAR_DIR_BA = 5;
int CAR_DIR_FW1 = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENA, OUTPUT); //ENA
  pinMode(IN1, OUTPUT); //IN1
  pinMode(IN2, OUTPUT); //IN2
  pinMode(ENB, OUTPUT); //ENB
  pinMode(IN3, OUTPUT); //IN3
  pinMode(IN4, OUTPUT); //IN4
  delay(2000);

}

void init_line_tracer_modules() {
  pinMode(LT_MODULE_L, INPUT);
  pinMode(LT_MODULE_F, INPUT);
  pinMode(LT_MODULE_R, INPUT);
}

bool LEFT(){
  int ret = analogRead(LT_MODULE_L);
  return (ret > 500) ? (true) : (false);
}

bool FORWARD(){
  int ret = analogRead(LT_MODULE_F);
  return (ret > 500) ? (true) : (false);
}

bool RIGHT(){
  int ret = analogRead(LT_MODULE_R);
  return (ret > 500) ? (true) : (false);
}

void It_mode_update(){
  bool ll = LEFT();
  bool ff = FORWARD();
  bool rr = RIGHT();

  if(ll){
    g_carDirection = CAR_DIR_LF;
  }
  else if(!ll && ff && rr){
    g_carDirection = CAR_DIR_FW1;
  }
  else if(!ll && ff && !rr){
    g_carDirection = CAR_DIR_FW;
  }
  else if(!ll && rr && !ff){
    g_carDirection = CAR_DIR_RF;
  }
  else if(!ll && !rr && !ff){
    g_carDirection = CAR_DIR_BA;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  It_mode_update();
  if(g_carDirection == CAR_DIR_LF){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
    delay(250);
    while(1){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      if(FORWARD()){
        return;
      }
    }
  }
  else if(g_carDirection == CAR_DIR_RF){
    while(!(FORWARD())){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, speed);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, speed);
    }
  }
  
  else if(g_carDirection == CAR_DIR_FW){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
  }
  else if(g_carDirection == CAR_DIR_BA){
    while(!(FORWARD()) && !(RIGHT())){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      if(FORWARD()){
        return;
      }
    }
  }
  else if(g_carDirection == CAR_DIR_FW1){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed);
    delay(50);
    int x = 0;
    while(x < 15){
      if(FORWARD()){
        return;
      }
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, speed);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, speed);
      x++;
      delay(10);
    }
    
      while(!(FORWARD())){
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, speed);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, speed);
      }
    
  }

}
