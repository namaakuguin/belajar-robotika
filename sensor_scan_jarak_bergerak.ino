#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
Servo myservo;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display (SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, -1);
const int trig = 12;
const int echo = 13;
const int buzzer = 17;

long duration;
float distance;

void setup(){
  myservo.attach(15);
  pinMode (trig,OUTPUT);
  pinMode (echo,INPUT);
  pinMode (buzzer, OUTPUT);
  Serial.begin (9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC,0x3c)){
    Serial.println (F("GAGAL MEMBACA SENSOR"));
    for(;;);
  }
}


float baca_jarak(){
  digitalWrite (trig, LOW);
  delayMicroseconds(2);
  digitalWrite (trig,HIGH);
  delayMicroseconds(10);
  digitalWrite (trig,LOW);

  long duration = pulseIn(echo, HIGH);
  return (duration * 0.0343) / 2;
}

void scanDanKirim(int sudutAktif){
    myservo.write(sudutAktif);
    delay (60);
    distance = baca_jarak();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor (0,5);
    display.print ("Sudut: "); display.print (sudutAktif); display.print (" deg");
    display.setCursor (0,20);
    display.print ("Jarak: "); display.print (distance); display.print (" cm");
    if (distance <50){
      display.setCursor (0,40);
      display.println ("AWAS BAHAYA!");
      tone(buzzer, 1000);
    }else {
      display.setCursor (0,40);
      display.println ("AMAN");
      noTone (buzzer);
    }
    display.display();
}

void loop (){
  for (int sudut = 0; sudut <= 180; sudut +=5){
    scanDanKirim(sudut);
  }

  for (int sudut = 180; sudut >=0; sudut -=5){
    scanDanKirim(sudut);
  }
}