#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,20,4);
const int ldr = 34;
const int buzzer = 16;

const int batasgelap = 1500;

void setup(){
  Serial.begin (115200);
  lcd.init();
  lcd.backlight();
  pinMode (buzzer,OUTPUT);
}



void loop(){
  int sensor = analogRead(ldr);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print ("-SENSOR PENDETEKSI-");
  lcd.setCursor (0,1);
  lcd.print ("LDR NOW: ");
  lcd.print (sensor);
  if (sensor <batasgelap){
    lcd.setCursor (0,3);
    lcd.print ("Terdeteksi Orang!");
    tone (buzzer,1000);
    delay(200);
    tone (buzzer,1500);
    delay(200);
  }else{
    noTone(buzzer);
    delay (1000);
  }
}