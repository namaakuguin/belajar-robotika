from machine import Pin, PWM, SoftI2C
from time import sleep
from ssd1306 import SSD1306_I2C
from mpu6050 import accel
import math

button = Pin(0, Pin.IN, Pin.PULL_UP)
i2c = SoftI2C(scl = Pin(16), sda = Pin(17))
sensor = accel(i2c)
oled = SSD1306_I2C(128,64,i2c,0X3C)
buzzer = PWM(Pin(18), freq = 1000)
buzzer.duty(0)
led_red = Pin(13, Pin.OUT)
led_blue = Pin(12, Pin.OUT)
led_green = Pin(14, Pin.OUT)

def color(rojo, azul, verde):
  led_red.value(rojo)
  led_blue.value(azul)
  led_green.value(verde)

def bip(brightness):
  if brightness == 0:
    buzzer.duty(0)
  else:
    buzzer.duty(512)
    sleep(brightness)
    buzzer.duty (0)
    sleep(brightness)

aktif = False

while True:
  if button.value() == 0:
    aktif = not aktif
    sleep(0.3)
  if aktif :
    data = sensor.get_values()
    ax = float(data['AcX'])
    ay = float(data['AcY'])
    az = float(data['AcZ'])

    if az == 0:
      az = 1.0

    sudut_x = math.atan2(ax,az) * 57.2958
    sudut_y = math.atan2(ay,az) * 57.2958

    x = abs(sudut_x)
    y = abs(sudut_y)

    oled.poweron()
    oled.fill(0)
    oled.text ("Miring:" + str(round(sudut_x)), 0,0)
    oled.text ("MiringDepan:" + str(round(sudut_y)), 0, 16)

    if x <30 and y <30:
      oled.text("aman", 0, 32)
      buzzer.duty(0)
      color (0,0,1)
    elif x <60 and y <60:
      oled.text ("terlalu miring!", 0, 32)
      bip(0.7)
      color (0,1,1)
    elif x <80 and y <80:
      oled.text ("peringatan jatuh!", 0, 32) 
      bip(0.3)
      color(1,1,0)
    else:
      oled.text ("mampus lu!",0, 32)
      color(1,0,0)

      buzzer.duty(512)
      sleep(0.1)
    oled.show()
  else:
    oled.poweroff()
    color(0,0,0)
    buzzer.duty(0)
    sleep(0.1)