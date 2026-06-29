#include <WiFi.h> 
//Memanggil pustaka bawaan untuk mengaktifkan fungsi Wi-Fi pada chip ESP32 (menyalakan antena internalnya).
#include <HTTPClient.h>
//Memanggil pustaka yang berfungsi seperti web browser mini, membuat ESP32 bisa membuka alamat website atau URL API di internet.


const char * ssid = "Wokwi-GUEST";
const char * password = "";

String botToken = "id dari @botfather";
String chatId = "1id dari @userifobot";
//Variabel untuk menyimpan koordinat tujuan pengiriman pesan di server Telegram

void KirimPesanTelegram (String pesan){
  if (WiFi.status() == WL_CONNECTED){ //Sebelum mengirim data, ESP32 akan mengecek statusnya terlebih dahulu. Jika statusnya beneran terhubung (WL_CONNECTED), perintah di dalam kurung kurawal baru akan dijalankan.
    HTTPClient http;

    pesan.replace (" ", "%20"); //Di dalam aturan URL internet, sebuah tautan tidak boleh mengandung karakter spasi kosong. Perintah ini otomatis mengubah semua spasi di teks lu menjadi %20 (kode baca spasi untuk browser) agar sistem tidak eror.

    String url = "https://api.telegram.org/bot" + botToken + "/sendMessage?chat_id=" + chatId + "&text=" + pesan;
//Baris ini adalah proses merakit teks menjadi satu kesatuan tautan URL. Semua variabel (Token, Chat ID, dan Isi Pesan) digabungkan menjadi satu alamat web panjang yang siap ditembak.

    http.begin (url) ;
    int httpResponeCode = http.GET(); //ESP32 menekan tombol "Enter" atau memicu tautan tersebut. Hasil respons dari server Telegram (apakah sukses atau gagal) akan disimpan dalam variabel angka bernama httpResponeCode.

    if (httpResponeCode >0){
      Serial.print ("HTTP Respone Code: ");
      Serial.println (httpResponeCode);
      String payload = http.getString();
      Serial.println ("Respone dari telegram : " + payload);
    }else {
      Serial.print ("Error saat mengirim data");
      Serial.println (httpResponeCode);
    } //Jika httpResponeCode bernilai di atas 0 (seperti angka 200), berarti server merespons. http.getString() mengambil teks balasan dari server Telegram untuk dicetak ke Serial Monitor sebagai bukti laporan.
  http.end(); 
  //Menutup koneksi internet yang tadi dibuka agar memori ESP32 kembali lega dan tidak boros baterai/daya.
  } else {
    Serial.println ("Koneksi Wifi Terputus");
  }
}

void setup(){
  Serial.begin (115200);
  WiFi.begin (ssid, password); //Memerintahkan chip Wi-Fi ESP32 untuk mulai mencari dan melakukan koneksi ke hotspot Wokwi.
  while (WiFi.status () != WL_CONNECTED){
    delay (500); //Baris ini mengunci program agar tidak lanjut ke bawah selama status Wi-Fi belum terhubung (!= WL_CONNECTED). Selama nunggu, dia bakal mencetak tanda titik . ke monitor setiap 0,5 detik sebagai indikator loading.
    Serial.print ("");
  }

  Serial.println ("\nWifi terhubung"); //Begitu lolos dari jebakan while (artinya Wi-Fi sudah terkoneksi), program akan memanggil fungsi kustom KirimPesanTelegram yang sudah kita buat di atas tadi dan melempar teks di dalam tanda kurung ke HP.
  delay (1000);

  Serial.println ("Mencoba mengirimkan pesan ke telegram...");
  KirimPesanTelegram ("Halo guin , perkenalkan namaku guin :D");
}

void loop(){
//Karena kita meletakkan fungsi kirim pesan di dalam setup(), bagian loop() sengaja dikosongkan agar ESP32 tidak mengirimkan chat yang sama jutaan kali ke akun Telegram tanpa henti.
}