#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,16,2);
#include <RTClib.h>
RTC_DS3231 rtc;

const int ledred = 12;
const int ledgreen = 13;
const int ledblue = 14;
const int sensor = 27;

void led (int rojo,int verde,int azul){
  digitalWrite (ledred,rojo);
  digitalWrite (ledgreen,verde);
  digitalWrite (ledblue, azul);
}

void setup (){
  pinMode (ledred, OUTPUT);
  pinMode (ledgreen,OUTPUT);
  pinMode (ledblue, OUTPUT);
  pinMode (sensor , INPUT);
  lcd.init();
  lcd.backlight();
  rtc.begin ();
  rtc.adjust (DateTime(F(__DATE__),F(__TIME__)));
  Serial.begin (9600);
}

void loop (){
  int gerak = analogRead (sensor);
  DateTime now = rtc.now();
  if (gerak >2500){
    led(1,0,0);
    lcd.setCursor(0,0);
    lcd.print ("CAHAYA TERANG!  ");
  }else if (gerak >800){
    led(1,1,0);
    lcd.setCursor(0,0);
    lcd.print ("CAHAYA REDUP    ");
  }else {
    led(0,1,1);
    lcd.setCursor (0,0);
    lcd.print ("GELAP          ");
  }
  lcd.setCursor(0,1);
  lcd.print (now.hour());
  lcd.print (":");
  lcd.print (now.minute());
  lcd.print (":");
  lcd.print (now.second());
  lcd.print ("    ");
  delay (300);
}