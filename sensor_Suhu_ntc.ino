#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire ,-1);
const int ntc = 32;
const float BETA = 3950;
const int buzzer = 0;

void setup(){
  Serial.begin (9600);
  analogReadResolution (12);
  if (!display.begin (SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println(F("ERROR GAGAL MEMBACA!"));
    for (;;);
  }
}

void loop(){
  int analogValue = analogRead(ntc);
  if (analogValue == 0) analogValue = 1;
  float celsius = 1 / (log(1/(4095.0 / analogValue - 1))/ BETA + 1.0 / 298.15) - 273.15;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor (0,5);
  display.print ("suhu : ");
  display.print (celsius);
  display.print ("C");
  if (celsius >45){
    tone(buzzer, 1000);
    display.setCursor (0,20);
    display.print("PANAS TINGGI!");
  }else if (celsius >35){
    noTone(buzzer);
    display.setCursor (0,20);
    display.print("SUHU MULAI PANAS!");
  }else{
    noTone(buzzer);
    display.setCursor(0,20);
    display.print ("AMAN TERKENDALI");
  }
  display.display();
  delay (1000);
}

