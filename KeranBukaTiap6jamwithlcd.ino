#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,20,4);
#include <RTClib.h>
RTC_DS3231 rtc;
#include <ESP32Servo.h>
const int buzzer = 12;
Servo myservo;
bool keranBuka = false;

void setup(){
  lcd.init();
  lcd.backlight();
  myservo.attach (13);
  pinMode (buzzer, OUTPUT);
  rtc.begin ();
  rtc.adjust (DateTime(F(__DATE__),F(__TIME__)));
}

void loop(){
  DateTime now = rtc.now();

  int jam = now.hour();
  int menit = now.minute();
  int detik = now.second();

  char bufferWaktu[10];
  sprintf(bufferWaktu, "%02d:%02d:%02d", jam, menit, detik);

  lcd.setCursor(0, 0);
  lcd.print("Jam: ");
  lcd.print(bufferWaktu);
  lcd.print("   ");

  if ((jam == 6 || jam == 12 || jam == 18) && menit == 0 && detik ==0 && !keranBuka){
    myservo.write (90);
    tone (buzzer, 1000);
    delay (200);
    noTone (buzzer);
    lcd.setCursor (0,2);
    lcd.print ("Keran Terbuka ");
    keranBuka = true;
  }
  if ((jam == 6 || jam == 12 || jam == 18) && menit == 0 && detik == 25 && keranBuka){
    myservo.write (0);
    noTone (buzzer);
    lcd.setCursor (0,2);
    lcd.print ("Keran Tertutup");
    keranBuka = false;
  }
  delay (100);
}