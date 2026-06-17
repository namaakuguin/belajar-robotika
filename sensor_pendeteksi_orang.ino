#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 20, 4);

const int led = 12;
const int buzzer = 4;
const int sensor = 13;
const int button = 2;

bool tombol = false;

void bip(float jeda){
  tone (buzzer, 1000);
  delay ( jeda *1000);
  noTone (buzzer);
  delay (jeda *1000);
}

void setup (){
  lcd.init ();
  lcd.backlight ();
  pinMode (led, OUTPUT);
  pinMode (buzzer, OUTPUT);
  pinMode (sensor, INPUT);
  pinMode (button, INPUT_PULLUP);
  Serial.begin (9600);
}

void loop(){
  int menyala = digitalRead (button);

  if (menyala == LOW){
    tombol = !tombol;
    delay (300);
  }

  if (tombol == true){
    delay (2000);
    lcd.clear();
    int gerak = digitalRead (sensor);

    if(gerak == HIGH){
      digitalWrite (led,HIGH);
      bip(0.3);
      lcd.setCursor(0,0);
      lcd.print ("PERINGATAN!");
      lcd.setCursor (0,1);
      lcd.print ("TERDETEKSI ADA ORANG");
    }else{
      digitalWrite (led,LOW);
      noTone (buzzer);
      lcd.setCursor (0,0);
      lcd.print ("SITUASI AMAN");
    }
  }else{
    lcd.clear();
    noTone(buzzer);
    digitalWrite (led,LOW);
  }
}