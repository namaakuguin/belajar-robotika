from machine import Pin, SoftI2C
from time import sleep
import dht
import ssd1306

#konfigurasi Pin utama
i2c = SoftI2C(scl = Pin(23), sda = Pin(22))
oled = ssd1306.SSD1306_I2C(128,64,i2c,0X3C)
sensor = dht.DHT22(Pin(12))
button = Pin(2, Pin.IN, Pin.PULL_UP)
led_rojo = Pin(14, Pin.OUT)
led_verde = Pin(16, Pin.OUT)
led_azul = Pin(17, Pin.OUT)

#definisi untuk led rgb
def color (rojo,verde,azul):
  led_rojo.value(rojo)
  led_verde.value(verde)
  led_azul.value(azul)

#definisi untuk memberi tau definisi dari aktif
aktif = False

while True:
  try :
    if button.value() == 0:  #untuk mengaktifkan fungsi tombol
      aktif = not aktif
      sleep(0.1)
    if aktif : #jika tombol ditekan maka dia akan menjalankan proses dibawah
      sensor.measure() #baca sensor suhu
      suhu_sekarang = sensor.temperature() #untuk membaca suhu sekarang
      oled.poweron() #untuk mengaktifkan oled
      #untuk mencetak kondisi suhu sekarang di oled maka dijelaskan dengan bahasa dibawah
      oled.fill(0)
      oled.text("suhu sekarang :" ,0,0)
      oled.text(str(round(suhu_sekarang)) + "C" ,0,16)
      if suhu_sekarang >40:
        color(1,0,0)
        oled.text ("Terlalu Panas!",0,32)
      elif suhu_sekarang <10:
        color(0,0,1)
        oled.text ("Terlalu Dingin", 0,32)
      else:
        color(0,1,0)
        oled.text("Suhu Aman",0,32)
      oled.show()
    else: #dilakukan untuk menonaktifkan semua fungsi sensor oled dan led agar mati jika ditekan tombol lagi
      oled.poweroff()
      color(0,0,0)
      sleep(2)
  except OSError as e: #untuk mencegah error yang menyebabkan crash
    oled.fill(0)
    oled.text("Erorr!",0,0)
    oled.show
    sleep(2)
