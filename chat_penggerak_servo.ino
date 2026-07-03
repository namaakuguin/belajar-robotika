#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>
#include <ESP32Servo.h>

LiquidCrystal_I2C lcd (0x27, 20,4);
Servo myservo;

const int servo = 17;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
String botToken = "codebottoken";

WiFiClientSecure client ;
UniversalTelegramBot bot (botToken, client);

unsigned long waktuterakhircek;
const unsigned long intervalcek = 2000;

void handlenewmessages (int numnewmessages){
  for (int i = 0; i<numnewmessages; i++){
    String text = bot.messages[i].text;
    Serial.println ("cmd : "+text);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print ("pesan dari telegram : ");

    if (text == "servo0"){
      myservo.write (0);
      lcd.setCursor (0,1);
      lcd.print ("servo bergerak => 0");
    }
    else if (text == "servo90"){
      myservo.write (90);
      lcd.setCursor (0,1);
      lcd.print ("servo bergerak => 90");
    }
    else if (text == "servo180"){
      myservo.write (180);
      lcd.setCursor (0,1);
      lcd.print ("servo bergerak => 180");
    }else {
      lcd.setCursor(0,1);
      lcd.print ("Pesan Biasa:");
      if (text.length() >0){
        lcd.setCursor (0,2);
        lcd.print (text.substring(0,20));
      }
      if (text.length() >20){
        lcd.setCursor (0,3);
        lcd.print (text.substring(20,40));
      }
    }
  }
}

void setup (){
  lcd.init();
  lcd.backlight();
  Serial.begin (115200);
  myservo.attach (servo,500,2000);
  myservo.write (0);

  lcd.setCursor (0,1);
  lcd.print ("connect to wifi...");

  WiFi.begin (ssid,password);
  client.setInsecure();

  while (WiFi.status() != WL_CONNECTED){
    delay (500);
    Serial.print(".");
  }

  Serial.print ("/Wifi tersambung!");

  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print ("Wifi tersambung");
  lcd.setCursor (0,1);
  lcd.print ("Siap kontrol servo");
}

void loop(){
  if (millis() > waktuterakhircek + intervalcek){
    int numnewmessages = bot.getUpdates(bot.last_message_received +1);
    while (numnewmessages){
      Serial.println ("ada pesan baru!");
      handlenewmessages(numnewmessages);
      numnewmessages = bot.getUpdates (bot.last_message_received +1);
    }
    waktuterakhircek = millis();
  }
}