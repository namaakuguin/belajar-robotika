#include <ESP32Servo.h>
#include <Keypad.h>
const int servo = 15;
const int buzzer = 16;
Servo pintolock ;

const byte ROWS = 4;
const byte COLS = 4;
char keys [ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins [ROWS] = {13,12,14,27};
byte colsPins [COLS] = {26,25,33,32};

Keypad keypad =Keypad (makeKeymap(keys),rowPins,colsPins,ROWS,COLS);

String inputPin = "";
const String pinBenar = "2134" ;
int salahSistem = 0;

void setup(){
  Serial.begin (115200);
  pinMode (buzzer,OUTPUT);
  pintolock.attach (servo,500,2400);
  pintolock.write (0);
  lobby();
}

void loop(){
  char key = keypad.getKey();
  if (key){
    tone (buzzer, 2000,50);
    if (key == '#'){
      if (inputPin == pinBenar){
        aksesDiterima();
      }else{
        salahSistem ++;
        aksesDitolak();
      }
    }else if (key == '*'){
      inputPin = "";
      lobby();
    }else {
      if (inputPin.length() <4){
        inputPin += key;
      }
    }
  }
}

void lobby(){
  Serial.println ("Tekan Pin yang benar");
  Serial.println ("Maksimal 4 karakter");
  Serial.println ("Jika salah 3x maka alarm akan menyala!");
  Serial.println ("#ENTER | *RESET");
}

void aksesDiterima(){
  Serial.println ("PIN BENAR");
  pintolock.write (90);
  salahSistem = 0;
  delay (5000);
  pintolock.write (0);
  inputPin = "";
  lobby();
}

void aksesDitolak(){
  Serial.println ("PIN SALAH!");
  if (salahSistem >= 3){
    Serial.println ("MALINGGGGG");
    for (int i =0; i <10; i++){
      tone (buzzer,800);
      delay (300);
      tone (buzzer,1200);
      delay (300);
    }
    noTone (buzzer);
    salahSistem = 0;
  }else{
    tone (buzzer, 400,1000);
    delay (1000);
  }
  inputPin = "";
  lobby();
}