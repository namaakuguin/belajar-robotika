#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

const int trig = 14;
const int echo = 12;

bool notifikasi = false;
long duration ;
float distance ;

const char* ssid = "Wokwi-GUEST";
const char* password =  "";

String botToken = "kodebottoken";
String chatId = "kodechatid";

float baca_jarak (){
  digitalWrite (trig,LOW);
  delayMicroseconds (2);
  digitalWrite (trig,HIGH);
  delayMicroseconds (10);
  digitalWrite (trig,LOW);
  long duration = pulseIn(echo,HIGH);
  return (duration * 0.0343)/ 2;
}

void kirimPesan (String pesan){
  if (WiFi.status() == WL_CONNECTED){
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    pesan.replace  (" ", "%20");

    String url = "https://api.telegram.org/bot"+ botToken + "/sendMessage?chat_id="+ chatId + "&text=" + pesan;

    http.begin(client,url);

    int httpResponeCode = http.GET();

    if (httpResponeCode >0){
      Serial.print ("HTTP RESPONE CODE:");
      Serial.println (httpResponeCode);
      String payload = http.getString();
      Serial.println ("respone dari telegram :"+ payload);
    }else {
      Serial.print ("Error saat mengirim data:");
      Serial.println (httpResponeCode);
    }
    http.end ();
  }else{
    Serial.print ("Koneksi terputus!");
  }
}

void setup(){
  Serial.begin (115200);
  WiFi.begin (ssid,password);
  pinMode (trig,OUTPUT);
  pinMode (echo,INPUT);

  while (WiFi.status () != WL_CONNECTED){
    Serial.print ("menghubungkan ke wifi...");
    delay (500);
  }

  Serial.println ("\nWifi terhubung");
  delay (1000);


}

void loop (){
  distance = baca_jarak();
  Serial.print ("jarak terdeteksi objek saat ini :");
  Serial.print (distance);
  Serial.println (" cm");
  if (distance <20 && !notifikasi){
    Serial.println ("sedang mengirim pesan...");
    kirimPesan ("Awas objek sangat dekat, terdeteksi : " + String(distance) +"cm");
    notifikasi = true;
  }
  if (distance >= 20 && notifikasi){
    Serial.println ("sedang mengirim pesan...");
    kirimPesan ("Objek dalam jarak batas aman :D");
    notifikasi = false;
  }
  delay (1000);
}