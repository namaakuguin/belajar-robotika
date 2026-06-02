from machine import Pin,PWM,SoftI2C
from time import sleep
from mpu6050 import accel

buzzer = PWM(Pin(25), freq = 1000)
led = Pin(4, Pin.OUT)
buzzer.duty(0)
i2c = SoftI2C(scl = Pin(18), sda = Pin(19))
sensor = accel(i2c)
button = Pin(15, Pin.IN, Pin.PULL_UP)

def bip(brightness):
  if brightness == 0:
    buzzer.duty(0)
  else:
    buzzer.duty(512)
    sleep(brightness)
    buzzer.duty(0)
    sleep(brightness)

def kedip(kecepatan):
  led.value (1)
  sleep(kecepatan)
  led.value(0)
  sleep(kecepatan)

aktif = False

while True:
  if button.value() == 0:
    aktif = not aktif
    sleep(0.3)
  if aktif :
    data = sensor.get_values()
    print ("aklerasi", data['AcX'], data['AcY'], data['AcZ'])
    print ("gyroscope", data ['GyX'], data ['GyY'], data ['GyZ'])
    sleep(0.5)
    if data['AcX'] >5000:
      bip(0.2)
      kedip(0.2)
      print ("Terlalu Miring Kanan bang")
    elif data['AcX'] <-5000:
      bip(0.5)
      kedip(0.5)
      print ("Terlalu Miring Kiri Bang")
    else:
      buzzer.duty(0)
      led.value(0)
      print ("Aman Cuy")
  else:
    buzzer.duty(0)
    led.value(0)