from machine import Pin,PWM,time_pulse_us,SoftI2C
from time import sleep
from mpu6050 import accel

led = PWM(Pin(13), freq = 1000)
buzzer = PWM(Pin(19), freq = 1000)
buzzer.duty(0)
button = Pin(2, Pin.IN, Pin.PULL_UP)
trig = Pin(5, Pin.OUT)
echo = Pin(18, Pin.IN)
i2c = SoftI2C(scl=Pin(4) , sda = Pin(16))
sensor = accel (i2c)

def baca_jarak():
  trig.value(0)
  sleep(0.000002)
  trig.value(1)
  sleep(0.00001)
  trig.value(0)
  durasi = time_pulse_us(echo, 1, 30000)
  jarak = durasi/ 58
  return jarak

def bip(brightness):
  if brightness == 0:
    buzzer.duty(0)
  else :
    buzzer.duty(512)
    sleep(brightness)
    buzzer.duty(0)
    sleep(brightness)

aktif = False

while True:
  if button.value() == 0:
    aktif = not aktif
    sleep(0.3)

  if aktif :
    jarak = baca_jarak()
    print ("jarak objek", jarak, "cm")
    data = sensor.get_values()
    print ("accelometer", data ['AcX'], data ['AcY'], data['AcZ'])
    print ("gyroscope", data['GyX'], data['GyY'], data ['GyZ'])
    sleep(0.5)
    if data['AcX'] >5000:
      print("Terlalu miring kanan bro")
    elif data ['AcX'] <-5000:
      print("Terlalu miring kiri bro")
    else:
      print("aman santuy")
    if jarak <=0 or jarak >400:
      continue
    if jarak < 10:
      intensitas, suara = 1023, 0.2
    elif jarak < 50:
      intensitas, suara = 512,0.5
    elif jarak < 100:
      intensitas, suara = 100, 1
    else:
      intensitas, suara = 0,0

    led.duty(intensitas)
    bip(suara)

    if suara == 0:
      sleep(0.1)
  else:
    led.duty(0)
    bip(0)