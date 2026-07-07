#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht (DHTPIN, DHTTYPE);

const char * ssid = "Wokwi-GUEST";
const char * password = "";

String botToken = "kodedribottoken";
String chatId = "kodedarichatid";

bool notifikasiTerkirim = false;

void KirimPesan (String pesan){
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    pesan.replace ("", "%20");

    String url = "https://api.telegram.org/bot"+ botToken + "/sendMessage?chat_id="+ chatId + "&text=" + pesan;

    http.begin(url);
    int httpResponeCode = http.GET();

    if (httpResponeCode >0){
      Serial.print ("HTTP RESPONE CODE:");
      Serial.println (httpResponeCode);
      String payload = http.getString();
      Serial.println ("Respone code: "+payload);
    }else{
      Serial.print ("Error saat mengirim data");
      Serial.println (httpResponeCode);
    }
  http.end();
  }else{
    Serial.println ("Koneksi Wifi Terputus");
  }
}

void setup(){
  Serial.begin (115200);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay (500);
    Serial.print ("");
  }
  dht.begin();
  Serial.println ("\nWifi terhubung");
}

void loop(){
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan (suhu) || isnan (kelembapan)){
    Serial.println (F("Gagal Membaca Suhu"));
    return ;
  }

  if (suhu <20 && !notifikasiTerkirim){
    KirimPesan ("Ini berlebihan guin terlalu dingin!");
    notifikasiTerkirim = true;

  } 
  if (suhu >=20 && notifikasiTerkirim){
    KirimPesan ("AMAN MIN! SANTUY AE? jangan sampe gosong cuy");
    notifikasiTerkirim = false;
  }
  delay(500);
}
