#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi objek untuk LCD 16x2 (gunakan alamat I2C LCD Anda)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin digital untuk tombol antrian dan teller
const int queueButtonPin = A0;    // Pin A0 sebagai input untuk tombol antrian
const int tellerButtonPin = A1;   // Pin A1 sebagai input untuk tombol teller
const int buzzerPin = 8;          // Pin 8 sebagai output untuk buzzer

// Variabel untuk menyimpan nomor antrian, nomor teller, dan queue karcis
int queueNumber = 0;   // Nomor Antrian maks 20
int tellerNumber = 0;  // Inisialisasi nomor teller mulai dari 0
int queueTicket = 0;   // Queue karcis saat ini

// Variabel untuk menyimpan status tombol dan waktu terakhir tombol ditekan
int lastQueueButtonState = HIGH;
int lastTellerButtonState = HIGH;

// Fungsi untuk mereset nomor teller menjadi 1
void resetTeller() {
  tellerNumber = 1;
}

// Fungsi untuk mengeluarkan suara bel
void playBel() {
  tone(buzzerPin, 1000, 500); // Frekuensi 1000 Hz (suaranya lebih rendah) dengan durasi 500 ms
  delay(500); // Jeda 500 ms antara bunyi bel
  noTone(buzzerPin);
}

// Fungsi untuk mengeluarkan bunyi gagal atau penolakan
void playPenolakan() {
  tone(buzzerPin, 600, 300); // Frekuensi 600 Hz dengan durasi 300 ms (bunyi pertama)
  delay(400); // Jeda 400 ms sebelum bunyi selanjutnya
  tone(buzzerPin, 300, 500); // Frekuensi 300 Hz dengan durasi 500 ms (bunyi kedua)
  delay(500); // Jeda 500 ms antara bunyi penolakan dan reset
  noTone(buzzerPin);
}

void setup() {
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.print("Antrian No: -");
  lcd.setCursor(0, 1);
  lcd.print("Teller -");

  // Inisialisasi pin sebagai input dengan pull-up resistor internal
  pinMode(queueButtonPin, INPUT_PULLUP);
  pinMode(tellerButtonPin, INPUT_PULLUP);

  // Inisialisasi pin buzzer sebagai output
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Baca status tombol antrian
  int queueButtonState = digitalRead(queueButtonPin);

  // Periksa perubahan status tombol antrian
  if (queueButtonState != lastQueueButtonState && queueButtonState == LOW) {
    if ((queueNumber - queueTicket) < 20) {
      queueNumber++;
      lcd.setCursor(12, 0);
      lcd.print("A");
      lcd.print(queueNumber);
      lcd.print(" ");
    } else {
      // Mainkan suara penolakan ketika antrian sudah penuh
      playPenolakan();
    }
  }
  lastQueueButtonState = queueButtonState;

  // Baca status tombol teller
  int tellerButtonState = digitalRead(tellerButtonPin);

  // Periksa perubahan status tombol teller
  if (tellerButtonState != lastTellerButtonState && tellerButtonState == LOW) {
    if (queueTicket < queueNumber) {
      tellerNumber++;
      if (tellerNumber > 4) {
        resetTeller(); // Reset nomor teller menjadi 1
      }
      queueTicket++; // Tambah queue karcis saat tombol teller ditekan
      lcd.setCursor(7, 1);
      lcd.print(tellerNumber);
      lcd.print(" - A");
      lcd.print(queueTicket); // Tampilkan queue karcis di sebelah angka teller
      
      // Mainkan suara bel ketika tombol teller ditekan
      playBel();
    }
  }
  lastTellerButtonState = tellerButtonState;

  // Tunggu sebentar sebelum mengulang
  delay(50);
}