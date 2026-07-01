#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
LiquidCrystal_I2C lcd (0x27,20,4);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

String botToken = "kodeBotToken";

WiFiClientSecure client;
UniversalTelegramBot bot (botToken, client);

unsigned long waktuterakhircek;
const unsigned long intervalCek = 2000;

void handleNewMessages (int numNewMessages){
  for (int i = 0; i <numNewMessages; i++){
    String text = bot.messages[i].text;

    Serial.println ("pesan diterima "+text);

    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print ("pesan dari telegram:");

    if (text.length() >0){
      lcd.setCursor (0,1);
      lcd.print (text.substring(0,20));
    }
    if (text.length() >20){
      lcd.setCursor (0,2);
      lcd.print (text.substring (20,40));
    }
    if (text.length() >40){
      lcd.setCursor (0,3);
      lcd.print (text.substring (40,60));
    }
  }
}

void setup (){
  lcd.init ();
  lcd.backlight ();
  Serial.begin (115200);
  lcd.setCursor (0,0);
  lcd.print ("connect to wifi...");

  WiFi.begin (ssid,password);
  client.setInsecure ();

  while (WiFi.status() != WL_CONNECTED){
    delay (500);
    Serial.print (".");
  }

  Serial.print ("\nWifi terhubung");

  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print ("wifi terhubung!");
  lcd.setCursor (0,1);
  lcd.print ("menunggu pesan...");
}

void loop (){
  if (millis() > waktuterakhircek + intervalCek){
    int numNewMessages = bot.getUpdates(bot.last_message_received +1);
  while (numNewMessages){
    Serial.println ("ada pesan baru!");
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received +1);
  }
  waktuterakhircek =millis();
  }
}
