#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd (0x27,20,4);
Servo pintolock;
const int servo = 2;
const int buzzer = 16;

const byte ROWS = 4;
const byte COLS = 4;
char keys [ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins [ROWS] = {13,12,14,27};
byte colPins [COLS] = {26,25,33,32};

Keypad keypad = Keypad (makeKeymap(keys),rowPins,colPins,ROWS,COLS);

String inputPin ="";
const String pinBenar = "2133"; //untuk ganti kode pinnya
int salahSistem = 0;

void setup(){
  Serial.begin (115200);
  lcd.init();
  lcd.backlight();
  pinMode (buzzer, OUTPUT);
  pintolock.attach (servo, 500, 2400);
  pintolock.write (0);
  tampilanAwal();
}

void loop(){
  char key = keypad.getKey();
  if (key){
    tone (buzzer, 2000,50); //untuk suara ketika keypadnya dipencet

    if (key == '#'){
      if (inputPin == pinBenar){
        aksesDiterima();
      }else {
        salahSistem++;
        aksesDitolak();
      }
    }
    else if (key == '*'){
      inputPin= "";
      tampilanAwal();
    }else{
      if (inputPin.length() < 4){
        inputPin += key;
        lcd.setCursor (6 + inputPin.length (),2);
        lcd.print ("*");
      }
    }
  }
}

void tampilanAwal (){ //untuk mendefinisikan tampilan awal saat baru menyala
  lcd.clear ();
  lcd.setCursor (3,0);
  lcd.print ("Sistem Keamanan");
  lcd.setCursor (4,1);
  lcd.print ("Sistem Rumah");
  lcd.setCursor (0,2);
  lcd.print ("PIN [    ]");
  lcd.setCursor (0,3);
  lcd.print ("#ENTER | *RESET");
}

void aksesDiterima(){ //ketika pin benar 
  lcd.clear();
  lcd.setCursor (3,1);
  lcd.print ("Akses Diterima");
  lcd.setCursor (4,2);
  lcd.print ("Silahkan Masuk");

  pintolock.write (90);
  salahSistem = 0;
  delay(5000);
  pintolock.write (0);
  inputPin ="";
  tampilanAwal();
}

void aksesDitolak (){ //ketika pin salah
  lcd.clear();
  lcd.setCursor (4,1);
  lcd.print ("Pin Salah");

  if (salahSistem >=3){ //ketika pin salah 3kali berturut turut maka alarm akan menyala
    lcd.setCursor (2,2);
    lcd.print ("Alarm Aktif!");
    for (int i =0; i < 10; i++){
      tone (buzzer, 800);
      delay (300);
      tone (buzzer,1200);
      delay (300);
    }
    salahSistem = 0;
  }else {
    tone (buzzer,400,1000);
    delay (1000);
  }
  inputPin = "";
  tampilanAwal();
}