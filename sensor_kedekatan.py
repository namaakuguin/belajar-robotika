
from machine import Pin,SoftI2C, time_pulse_us,PWM
from time import sleep
from ssd1306 import SSD1306_I2C

led = Pin(12, Pin.OUT)
buzzer = PWM(Pin(16), freq = 1000)
buzzer.duty(0)
button = Pin(2, Pin.IN, Pin.PULL_UP)
i2c = SoftI2C(scl = Pin(22), sda = Pin(23) )
oled = SSD1306_I2C (128, 64, i2c, 0x3C)
trig = Pin(18, Pin.OUT)
echo = Pin(5, Pin.IN)

aktif = False 

def baca_jarak():
  trig.value(0)
  sleep(0.000002)
  trig.value(1)
  sleep(0.00001)
  trig.value(0)
  durasi = time_pulse_us(echo, 1, 30000)
  jarak = durasi / 58
  return jarak

def bip(brightness):
  if brightness == 0:
    buzzer.duty(0)
  else:
    buzzer.duty(512)
    sleep(brightness)
    buzzer.duty(0)
    sleep(brightness)

while True:
  if button.value() == 0:
    aktif = not aktif
    sleep(0.3)
  if aktif:
    jarak = baca_jarak()
    if jarak <=0 or jarak >400:
      continue
    led.value(1)
    oled.poweron()
    oled.fill(0)
    oled.text ("jarak : " + str(round(jarak)) + "cm", 0 , 0)
    if jarak <50:
      oled.text ("kedekatan", 0, 16)
      bip(0.11)
    elif jarak <150:
      oled.text ("Cukup", 0, 16)
      bip(0.3)
    elif jarak <300:
      oled.text ("Pas bgt ini", 0, 16)
      bip(0.6)
    else:
      oled.text ("kejauhan", 0, 16)
      bip(1)
    oled.show()
  else:
    led.value(0)
    buzzer.duty(0)
    oled.poweroff()
```