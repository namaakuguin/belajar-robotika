#include <ESP32Servo.h>
Servo myservo;
const int ldr = 34;


void setup (){
  myservo.attach (14);
  Serial.begin(9600);
}

void loop (){
  int sensor = analogRead(ldr);
  Serial.println(sensor);
  delay (1000);
  if(sensor >2000){
    myservo.write (90);
    delay (2000);
  }else{
    myservo.write (0);
    delay(2000);
  }
}