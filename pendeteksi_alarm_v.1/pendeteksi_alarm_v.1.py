from machine import Pin, time_pulse_us, PWM
from time import sleep

led = PWM(Pin(13), freq = 1000)
buzzer = PWM(Pin(12), freq = 1000)
buzzer.duty(0)
trig = Pin(5, Pin.OUT)
echo = Pin(18, Pin.IN)
tombol = Pin(15, Pin.IN, Pin.PULL_UP)

def deteksi_jarak():
  trig.value (0)
  sleep(0.000002)
  trig.value (1)
  sleep (0.00001)
  trig.value(0)
  durasi = time_pulse_us(echo,1,30000)
  jarak = durasi/58
  return jarak

def bip (kecepatan):
  if kecepatan == 0:
    buzzer.duty(0)
  else:
    buzzer.duty(512)
    sleep(kecepatan)
    buzzer.duty(0)
    sleep(kecepatan)

aktif = False

while True:
  if tombol.value() == 0:
    aktif = not aktif
    sleep(0.3)
  if aktif:
    jarak = deteksi_jarak()
    if jarak <=0 or jarak >400:
      continue

    print("jarak terdeteksi", jarak, "cm")

    if jarak <10:
      val_led, jeda_bip = 1023,0.02
    elif jarak <50:
      val_led, jeda_bip = 512,0.4
    elif jarak <100:
      val_led, jeda_bip = 100,1
    else:
      val_led, jeda_bip = 0,0

    led.duty(val_led)
    bip(jeda_bip)

    if jeda_bip == 0:
      sleep(0.1)

  else:
    led.duty(0)
    buzzer.duty(0)
    sleep(0.1)