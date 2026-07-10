#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd (0x27,20,4);

const int led = 15;
const int trig = 13;
const int echo = 12;

const int tinggiTandon = 200;
const int airPenuh = 20;

const int batasBawah = 20;
const int batasAtas = 95;

bool statusPompa = false ;

int bacaJarak(){
  digitalWrite (trig, LOW);
  delayMicroseconds (2);
  digitalWrite (trig, HIGH);
  delayMicroseconds (10);
  digitalWrite (trig, LOW);
  long durasi = pulseIn (echo,HIGH);
  int jarak = durasi * 0.034 /2;
  return jarak;
}

void setup(){
  Serial.begin (115200);
  lcd.init();
  lcd.backlight();
  pinMode (trig, OUTPUT);
  pinMode (echo,INPUT);
  pinMode (led, OUTPUT);
  digitalWrite (led,LOW);
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print ("--MONITOR TANDON--");
}

void loop (){
  int jarakReal = bacaJarak();
  int persentaseAir = map (jarakReal, tinggiTandon, airPenuh, 0, 100);
  persentaseAir = constrain (persentaseAir , 0, 100);
  lcd.setCursor (0,1);
  lcd.print ("Volume air "+ String(persentaseAir) +"%   ");
  lcd.setCursor (0,2);
  lcd.print ("Status Pompa "+ String(statusPompa? "ON " : "OFF"));
  if (persentaseAir < batasBawah && !statusPompa){
    statusPompa= true;
    lcd.setCursor (0,3);
    lcd.print ("POMPA MENYALA!  ");
    digitalWrite (led,HIGH);
  }else if (persentaseAir >= batasAtas && statusPompa){
    statusPompa = false;
    lcd.setCursor (0,3);
    lcd.print ("POMPA DIMATIKAN!");
    digitalWrite (led,LOW);
  }
  delay (200);
}