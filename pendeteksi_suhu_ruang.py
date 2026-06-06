from machine import Pin,PWM,SoftI2C
from time import sleep
import dht
import ssd1306

# Konfigurasi pin utama 
led_red = Pin(14, Pin.OUT)
led_green = Pin(21, Pin.OUT)
led_blue = Pin(22, Pin.OUT)
buzzer = PWM(Pin(0), freq = 1000)
buzzer.duty(0)
i2c = SoftI2C(scl = Pin(4), sda = Pin(16))
oled = ssd1306.SSD1306_I2C(128, 64, i2c, 0X3C)
sensor_suhu = dht.DHT22(Pin(12))

# definisi untuk rgb led
def color(rojo,verde,azul):
  led_red.value(rojo)
  led_green.value(verde)
  led_blue.value(azul)

# definisi untuk bunyi buzzer
def bip(brightness):
  if brightness == 0:
    buzzer.duty(0)
  else:
    buzzer.duty(512)
    sleep(brightness)
    buzzer.duty(0)
    sleep(brightness)

while True:
  try:
    sensor_suhu.measure()

    suhu_sekarang = sensor_suhu.temperature() # memberi tau untuk memanggil suhu temperature

    oled.fill(0)
    oled.text ("Suhu Sekarang :",0, 0)
    oled.text (str(round(suhu_sekarang)) + " C", 0, 16)  # kode yang terus berubah untuk mendeteksi suhu ruang

# jika suhu panas diatas 30 derajat maka tampilan oled akan memberi tau dan buzzer berbunyi 0.3 sec serta lampu akan menyala warna merah
    if suhu_sekarang >30: 
      oled.text ("SUHU PANAS!", 0, 32)
      bip(0.3)
      color (1,0,0) 
# jika suhu dingin dibawah 0 derajat maka tampilan oled akan memberi tau dan buzzer berbunyi 0.7 sec serta lampu akan menyala warna biru
    elif suhu_sekarang <0:
      oled.text("SANGAT DINGIN",0,32)
      bip(0.7)
      color(0,0,1)
# jika suhu aman tidak terlalu panas dan tidak terlalu dingin maka tampilan oled akan memberi tau serta lampu akan menyala warna hijau
    else:
      oled.text("AMAN CUY",0,32)
      buzzer.duty(0)
      color (0,1,0)
    oled.show()
  except OSError as e:
    oled.fill(0)
    oled.text("Error!",0, 0)
    oled.show()

  sleep(2)

