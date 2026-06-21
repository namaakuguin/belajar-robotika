#include <RTClib.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,20,4);
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht (DHTPIN,DHTTYPE);
const int led = 12;

void setup (){
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode (led,OUTPUT);
  rtc.begin();
  rtc.adjust (DateTime (F(__DATE__),F(__TIME__)));
  Serial.begin (9600);
}

bool sudah_print = false;

void loop(){
  DateTime now = rtc.now();
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  lcd.setCursor (0,0);
  lcd.print (now.hour());
  lcd.print (":");
  lcd.print (now.minute());
  lcd.print (":");
  lcd.print (now.second());
  lcd.print ("           ");
  lcd.setCursor (0,1);
  lcd.print ("SUHU: ");
  lcd.print ((int)suhu);
  lcd.print ("C   ");
  lcd.setCursor (0,2);
  lcd.print ("KELEMBAPAN: ");
  lcd.print ((int)kelembapan);
  lcd.print ("%   ");
  if (now.second() == 0 && !sudah_print ){
    digitalWrite (led, HIGH);
    Serial.print ("[");
    Serial.print (now.hour());
    Serial.print (":");
    Serial.print (now.minute());
    Serial.print ("] SUHU:");
    Serial.print (suhu, 0);
    Serial.print ("C | kelembapan:");
    Serial.print (kelembapan,0);
    Serial.print ("%");
    Serial.println ();
    sudah_print = true;
  }else if (now.second() != 0) {
    digitalWrite (led,LOW);
    sudah_print = false;
  }
  delay (100);
}