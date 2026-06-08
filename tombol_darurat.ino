const int led_red=12;
const int led_green=16;
const int led_blue=17;
const int buzzer =5;
int nilaipwm=1000;
const int button = 2;

bool tombol_terpicu = false;

void led (int rojo, int verde, int azul){
  digitalWrite(led_red,rojo);
  digitalWrite(led_green,verde);
  digitalWrite(led_blue,azul);
}

void bip (float durasi) {
  tone(buzzer,1000);
  delay(durasi*1000);
  noTone(buzzer);
  delay(durasi*1000);
}

void setup (){
  pinMode(led_red,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_blue,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  Serial.begin(115200);
}



void loop (){
  int status_tombol = digitalRead(button);

  if (status_tombol == LOW){
    tombol_terpicu = !tombol_terpicu;
    delay(300);
  }

  if (tombol_terpicu == true){
    Serial.println("ALARM AKTIF!");
    Serial.println("SITUASI BAHAYA!");
    bip(0.2);
    led (1,0,0);
    delay(1000);
  }else{
    noTone(buzzer);
    led (0,1,0);
  }
}