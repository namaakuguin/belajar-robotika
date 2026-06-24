#include <ESP32Servo.h> // untuk memanggil library dari servo atau penggeraknya
#include <Adafruit_GFX.h> //library untuk memanggil library simonitor layar oled
#include <Adafruit_SSD1306.h> 
#define SCREEN_WIDTH 128 //mendefinisikan lebar dari layar oled tersebut dan dibawahnya untuk tingginya
#define SCREEN_HEIGHT 64
Servo myservo;
Adafruit_SSD1306 display (SCREEN_WIDTH,SCREEN_HEIGHT,&Wire, -1);
const int trig = 14; //pin-pin untuk sensor jarak 
const int echo = 12;

long duration;
float distance;

void setup (){
  myservo.attach(15); //memberi tau pin dari servo atau penggerak
  pinMode (trig,OUTPUT);
  pinMode (echo,INPUT);
  Serial.begin(115200);
  if (!display.begin (SSD1306_SWITCHCAPVCC, 0x3c)){ //jika terjadi error
    Serial.println (F("GAGAL MENDETEKSI SSD1306"));
    for (;;);
  }
}

void loop (){
  digitalWrite (trig ,LOW); 
  delayMicroseconds(2);
  digitalWrite (trig,HIGH);
  delayMicroseconds(10);
  digitalWrite (trig,LOW);

  duration = pulseIn (echo ,HIGH);
  distance = (duration * 0.0343) / 2;


  display.clearDisplay(); //membersihkan buffer oled disetiap awal loop
  display.setTextSize(1); //mengatur size ukuran text dalam oled
  display.setTextColor(WHITE); //mengatur warna text dalam oled

  if (distance <200){ //jika jarak sensor mendeteksi subjek atau objek dijarak 200cm kebawah
    myservo.write (0);
    display.setCursor(0,10);
    display.println("selamat datang"); //oled akan menampilkan text untuk menyambut objek atau subjek
    display.setCursor (0,25);
    display.println ("selamat menikmati :)");
  }else {
    myservo.write (90); //servo akan menutup kembali jika tidak ada objek atau subjek yg terdeteksi dijarak 200cm kebawah
  }
  display.display ();
  delay(500);
}