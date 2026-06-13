#include "DHT.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,20,4);
#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht (DHTPIN,DHTTYPE);

const int relay = 13;
const int button = 0 ;
const int buzzer = 4;
int pwm = 1000;

bool tombol = false;

void bip (float jeda){
  tone (buzzer, 1000);
  delay (jeda * 1000);
  noTone (buzzer);
  delay (jeda * 1000);
}

void setup(){
  lcd.init();
  lcd.backlight();
  dht.begin();
  Serial.begin (9600);
  pinMode (relay,OUTPUT);
  pinMode (buzzer, OUTPUT);
  pinMode (button, INPUT_PULLUP);
  digitalWrite (relay,LOW);
}

void loop (){
  int menyala = digitalRead (button);

  if (menyala == LOW){
    tombol = !tombol;
    delay (300);
  }

  if (tombol == true){
    delay (2000); 
    lcd.clear();


    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();

    if (isnan (suhu) || isnan (kelembapan)){
      Serial.print (F("GAGAL MEMBACA SENSOR!"));
      return;
    }else {
      lcd.setCursor (0,0);
      lcd.print ("SENSOR AKTIF!");
      lcd.setCursor (0,1);
      lcd.print (F("SUHU : "));
      lcd.print (suhu);
      lcd.print ("C");
      lcd.setCursor (0,2);
      lcd.print (F("KELEMBAPAN : "));
      lcd.print (kelembapan);
      lcd.print ("%");
    }

    if (suhu >37){
      lcd.setCursor (0,3);
      lcd.print ("SANGAT PANAS!");
      digitalWrite(relay,HIGH);
      bip(0.2);
    }
    else if (suhu <10){
      lcd.setCursor (0,3);
      lcd.print ("TERLALU DINGIN!");
      digitalWrite(relay,HIGH);
      bip(0.4);
    }else{
      noTone (buzzer);
      lcd.setCursor(0,3);
      lcd.print ("SUHU AMAN BANG");
      digitalWrite(relay, LOW);
    }
  }else {
    noTone(buzzer);
    lcd.clear();
    digitalWrite(relay,LOW);
  }
}