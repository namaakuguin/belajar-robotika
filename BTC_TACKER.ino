#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,-1);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* url = "https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT";

void setup (){
  Serial.begin (115200);
  if (!display.begin (SSD1306_SWITCHCAPVCC , 0x3c)){
    Serial.print (F("Gagal membaca layar oled"));
    for (;;);
  }

  display.clearDisplay ();
  display.setTextSize (1);
  display.setTextColor(WHITE);
  display.setCursor (0,20);
  display.println ("wifi conecting....");
  display.display();

  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print (".");
  }

  Serial.print ("WIFI TERHUBUNG");
  display.clearDisplay();
  display.setCursor (0,20);
  display.print ("WIFI CONNECTED");
  display.display();
  delay (1500);
  
}

void loop (){
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    http.begin (url);
    int httpResponeCode = http.GET();

    if (httpResponeCode >0){
      String payload = http.getString();
      Serial.println (payload);

      JsonDocument doc;
      DeserializationError error = deserializeJson(doc,payload);

      if (!error){
        const char* pricestr = doc["price"];
        float price = atof(pricestr);

        display.clearDisplay();

        display.setTextSize(1);
        display.setCursor (0,0);
        display.print ("BTC TICKER");
        display.drawFastHLine (0,12, 128, WHITE);

        display.setTextSize (2);
        display.setCursor (0,22);
        display.print ("BTC/USDT");

        display.setTextSize (1);
        display.setCursor (0,48);
        display.print ("$");
        display.print (price, 2);

        display.display();
      }else{
        Serial.print ("GAGAL Parse Json");
        Serial.println (error.c_str());
      }
    }else {
      Serial.print ("Error saat request http");
      Serial.println (httpResponeCode);
    }
    http.end();
  }else{
    Serial.print ("Koneksi Wifi Terputus");
  }
  delay (10000);
}