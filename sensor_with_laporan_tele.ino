#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
LiquidCrystal_I2C lcd (0x27,20,4);
const int ldr = 34;
const int buzzer = 16;

const int batasgelap = 1500;

const char * ssid = "Wokwi-GUEST";
const char * password = "";

String botToken = "botToken";
String chatId = "chatId"; //masukkan bottoken dan chatid telegram disini!

WiFiClientSecure client;
UniversalTelegramBot bot (botToken,client);

unsigned waktuTerakhirCek ;
const unsigned intervalCek = 2000;

void handleNewMessages (int numNewMessages){ //untuk perintah memanggil sensor dan mengirim pesan ke lcd fisik melewati chat
  for (int i=0; i<numNewMessages ; i++){
    String text = bot.messages[i].text;
    Serial.print ("cmd : "+text);
    int sensor = analogRead(ldr);
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print ("Pesan Dari Tele: ");
    if (text == "intensitas"){
      lcd.setCursor (0,1);
      lcd.print ("LDR NOW:");
      lcd.print (sensor);
      kirimPesanTelegram ("Intensitas saat ini :"+ String (sensor));
    }else{
      lcd.setCursor (0,1);
      lcd.print ("Pesan Biasa :");
      if (text.length() >0){
        lcd.setCursor (0,2);
        lcd.print (text.substring(0,20));
        lcd.setCursor (0,3);
        lcd.print (text.subString(20,40));
      }
    }
  }
}

void kirimPesanTelegram(String pesan){ //untuk membangun definisi untuk mengirim chat laporan balik ke chat
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    pesan.replace (" ", "%20");
    String url = "https://api.telegram.org/bot" +botToken + "/sendMessage?chat_id=" +chatId + "&text=" +pesan ;
    http.begin (url);
    int httpResponeCode = http.GET();
    if (httpResponeCode >0){
      Serial.print ("HTTP Respone code : ");
      Serial.println (httpResponeCode);
      String payload = http.getString();
      Serial.println ("respone dari telegram:" +payload);
    }else{
      Serial.print ("Error saat mengirim data");
      Serial.println (httpResponeCode);
    }
    http.end();
  }else {
    Serial.println ("Koneksi Wifi Terputus!");
  }
}

void setup(){ 
  Serial.begin (115200);
  lcd.init();
  lcd.backlight();
  pinMode (buzzer,OUTPUT);
  lcd.setCursor (0,1);
  lcd.print ("connect to wifi...");
  WiFi.begin (ssid,password);
  client.setInsecure();
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print (".");
  }
  Serial.print ("/WiFi tersambung");
  lcd.clear ();
  lcd.setCursor (0,0);
  lcd.print ("WiFi terhubung");
  lcd.setCursor (0,1);
  lcd.print ("Sensor aktif");
}



void loop(){ 
  if (millis ()> waktuTerakhirCek + intervalCek){
    int numNewMessages = bot.getUpdates (bot.last_message_received +1);
    while (numNewMessages){
      Serial.println ("ada pesan baru!");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates (bot.last_message_received +1);
    }
    waktuTerakhirCek =millis();
  }
  int sensor = analogRead(ldr); //alarm jika melewati batasgelap yang bertanda mendeteksi objek
  if (sensor <batasgelap){
    lcd.setCursor (0,3);
    lcd.print ("Terdeteksi Orang!");
    tone (buzzer,1000);
    delay (200);
    tone (buzzer,1500);
    delay (200);
  }else {
    noTone (buzzer);
    lcd.setCursor (0,3);
    lcd.print ("                  ");
  }
}