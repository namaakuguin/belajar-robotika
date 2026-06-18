const int led = 14;
const int ldr = 34;

void setup () {
  pinMode (led, OUTPUT);
  Serial.begin (9600);
}

void loop () {
  int sensor = analogRead (ldr);
  Serial.println (sensor);
  delay (300);
  if (sensor < 1000) {
    digitalWrite (led, HIGH);
    Serial.println ("TERANG-LED ON");
  } else {
    digitalWrite (led, LOW);
    Serial.println ("GELAP-LED OFF");
  }
}