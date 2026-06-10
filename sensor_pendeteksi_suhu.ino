#include "DHT.h"
#include <LiquidCrystal_I2C.h>   //konfigurasi lcdnya
LiquidCrystal_I2C lcd (0x27, 16, 2);
#define DHTPIN 13
#define DHTTYPE DHT22          //konfigurasi sensornya
DHT dht (DHTPIN,DHTTYPE);
// konfigurasi beberapa pin untuk buzzer, led dan tombolnya
const int buzzer = 17;
const int button = 2;
const int led_red = 12;
const int led_green = 5;
const int led_blue = 18;
int nilaipwm = 1000;

bool tombol = false;

//definisi untuk led rgb colornya
void color (int rojo,int verde,int azul){ 
  digitalWrite (led_red,rojo);
  digitalWrite (led_green,verde);
  digitalWrite (led_blue, azul);
}

//definisi untuk bunyi dan jeda buzzernya
void bip(float jeda){
  tone (buzzer, 1000);
  delay (jeda * 1000);
  noTone (buzzer);
  delay (jeda * 1000);
}

void setup () {
  pinMode (buzzer, OUTPUT);
  pinMode (led_red, OUTPUT);
  pinMode (led_green, OUTPUT);
  pinMode (led_blue, OUTPUT);
  pinMode (button, INPUT_PULLUP);
  Serial.begin (9600);
  dht.begin ();
  lcd.init ();
  lcd.backlight ();
}

void loop (){
  //untuk mengaktifkan fungsi tombol
  int status_tombol = digitalRead(button);

  if (status_tombol == LOW){
    tombol = !tombol;
    delay(300);
  }

  if (tombol == true){
    delay (2000); //atur ulang setiap dua detik
    lcd.clear();
    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();
    lcd.setCursor (0,0);

    if (isnan (suhu) || isnan (kelembapan)){ //peringatan jika gagal membaca suhu
      Serial.println (F("GAGAL MEMBACA SUHU"));
      return ;
    }else{
      lcd.setCursor (0,0); //untuk membaca kondisi suhu dan kelembapan saat ini
      lcd.print (F("T:"));
      lcd.print (suhu);
      lcd.print (F("|H:"));
      lcd.print (kelembapan);
    }

//mengatur kondisi suhu kepada layarnya
    if (suhu > 40){
      color (1,0,0);
      lcd.setCursor (0,1);
      lcd.print ("PANAS OVER");
      bip(0.2);
    }else if (suhu <10){
      color (0,0,1);
      lcd.setCursor(0,1);
      lcd.print ("DINGIN OVER");
      bip (0.4);
    }else {
      color (1,0,1);
      lcd.setCursor (0,1);
      lcd.print ("SUHU AMAN");
      noTone (buzzer);
    }
  }else{ //untuk mematikan sistemnya
    color (0,1,0);
    noTone (buzzer);
    lcd.clear();
  }
}