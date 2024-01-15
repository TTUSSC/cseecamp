#include <Servo.h>
#define SERVO_PIN 9
#define SW_PIN 8
#define INIT_DEG 10

Servo serv;

int deg=INIT_DEG;

void setup() {
  // put your setup code here, to run once:
  serv.attach(SERVO_PIN);
  pinMode(SERVO_PIN,OUTPUT);
  pinMode(SW_PIN,INPUT_PULLUP);
  Serial.begin(9600);

  serv.write(INIT_DEG);
}

void loop() {
  // put your main code here, to run repeatedly:
  deg=INIT_DEG;
  serv.write(INIT_DEG);
  delay(200);

  if(digitalRead(SW_PIN)==HIGH){
    Serial.println("is pulled.");
    while(digitalRead(SW_PIN)==HIGH && deg<=170){
      serv.write(++deg);
      delay(1);
    }
    //delay(100);
  }else Serial.println("is pushed.");
}
