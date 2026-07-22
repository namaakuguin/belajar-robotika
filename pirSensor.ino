#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 16,2);
const int pir = 2;
const int buzzer = 13;

void setup(){
  pinMode (pir, INPUT);
  pinMode (buzzer,OUTPUT);
  lcd.init();
  lcd.backlight();
}


void loop(){
  int bacaPir = digitalRead (pir); //panggil si sensor pir
  if (bacaPir == HIGH){ //jika pir mendeteksi gerakan
    lcd.setCursor (0,0);
    lcd.print ("ADA ORANG!"); //untuk menampilkan pesan di lcd
    tone (buzzer, 500); //bunyikan buzzer jika ada terdeteksi orang
    delay (5000); //kasi waktu 5 detik untuk mati
  }else{ //syntax untuk jika tidak terdeteksi gerakan
    noTone (buzzer);
    lcd.setCursor (0,0);
    lcd.print ("SEPI! AMAN");
  }
}