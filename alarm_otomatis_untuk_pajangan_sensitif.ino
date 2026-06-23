#include <ESP32Servo.h>
Servo myservo;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,16,2);
const int trig = 13;
const int echo = 12;
const int buzzer = 14;

long duration;
float distance;

void setup(){
  myservo.attach (15);
  lcd.init();
  lcd.backlight();
  pinMode (trig,OUTPUT);
  pinMode (echo, INPUT);
  pinMode (buzzer, OUTPUT);
  Serial.begin (9600);
}

void loop(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite (trig, HIGH);
  delayMicroseconds(10);
  digitalWrite (trig,LOW);

  duration = pulseIn (echo, HIGH);
  distance = (duration * 0.0343) / 2;

  Serial.print ("jarak saat ini:");
  Serial.print (distance);
  Serial.print (" cm");

  if (distance < 20){
    myservo.write (90);
    lcd.setCursor (0,0);
    lcd.print ("JAUHKAN TANGANMU!   ");
    lcd.setCursor (0,1);
    lcd.print ("DILARANG MENDEKAT   ");
    tone (buzzer , 1000);
  }else {
    myservo.write (0);
    lcd.setCursor (0,0);
    lcd.print ("DILARANG MENDEKAT   ");
    lcd.setCursor (0,1);
    lcd.print ("BENDA SENSITIF!     ");
    noTone (buzzer);
  }
  delay (500);
}