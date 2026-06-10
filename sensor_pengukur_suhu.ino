#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int led_red = 12;
const int led_green = 5;
const int led_blue = 18;
const int button = 2;
const int buzzer = 14;
int nilaipwm = 1000;

bool menyala = false;

void color (int rojo, int verde, int azul){
  digitalWrite(led_red,rojo);
  digitalWrite(led_green,verde);
  digitalWrite(led_blue,azul);
}

void bip (float jeda){
  tone (buzzer , 1000);
  delay (jeda * 1000);
  noTone (buzzer);
  delay (jeda *1000);
}

void setup (){
  pinMode (led_red, OUTPUT);
  pinMode (led_green, OUTPUT);
  pinMode (led_blue, OUTPUT);
  pinMode (button, INPUT_PULLUP);
  pinMode (buzzer, OUTPUT);
  Serial.begin (115200);
  Serial.println (F("Sensor Akan Menyala!"));
  dht.begin();
}

void loop (){
  int status_tombol = digitalRead(button);


  if (status_tombol == LOW){
    menyala = !menyala;
    delay(300);
  }



  if (menyala == true){
    delay (2000);

    float kelembapan = dht.readHumidity();

    float suhu = dht.readTemperature();

    if (isnan (kelembapan) || isnan (suhu)){
      Serial.println(F("GAGAL MEMBACA SUHU!"));
      return;
    } else{
      Serial.println ("PENGUKUR SUHU!");
      Serial.print ("Kelembapan: ");
      Serial.print (kelembapan);
      Serial.print ("% | Suhu: ");
      Serial.print (suhu);
      Serial.println (F("C"));
    }
      if (suhu >40){
        color (1,0,0);
        bip(0.2);
        Serial.println("PERINGATAN TERLALU PANAS!");
      } else if (suhu <10){
        color (0,0,1);
        bip(0.5);
        Serial.println("PERINGATAN TERLALU DINGIN!");
      }else{
        color (0,1,1);
        noTone(buzzer);
        Serial.println("SUHU AMAN");
      }
    }else{
      color(0,1,0);
      noTone(buzzer);
    }
}