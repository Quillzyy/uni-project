#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Ubah alamat sesuai dengan alamat LCD yang digunakan
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long mulai, selesai, dataStopWatch;
int i=0;
int fPaus = 0;
long lastButton = 0;
long delayAntiBouncing = 50;
long dataPaus = 0;

void setup(){
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  digitalWrite(A0,1);
  digitalWrite(A1,1);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  lcd.setCursor(0, 0); 
  lcd.print("Arduino Nih Bos");
  lcd.setCursor(0, 1); 
  lcd.print("   StopWatch");
  delay(2000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(" Tekan Tombol");
  lcd.setCursor(1, 1);
  lcd.print(" Start / Stop");
}

void loop(){
  if (digitalRead(A0)==0){
    if ((millis() - lastButton) > delayAntiBouncing){
      if (i==0){
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Start Timer");
        mulai = millis();
        fPaus = 0;
      }
      else if (i==1){
        lcd.setCursor(2, 0);
        lcd.print("Stop Timer ");
        dataPaus = dataStopWatch;
        fPaus = 1;
      }
      i =!i;
    }
    lastButton = millis();
  }
  else if (digitalRead(A1)==0 && fPaus == 1){
    dataStopWatch = 0;
    dataPaus = 0;
    lcd.clear();
    lcd.print("Reset Stopwatch");
    delay(2000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(" Tekan Tombol");
    lcd.setCursor(1, 1);
    lcd.print(" Start / Stop");
  }
  if (i==1){
    selesai = millis();

    float jam, menit, detik, miliDetik;
    unsigned long over;
    dataStopWatch = selesai - mulai;
    dataStopWatch = dataPaus + dataStopWatch;
    jam = int(dataStopWatch / 3600000);
    over = dataStopWatch % 3600000;
    menit = int(over / 60000);
    over = over % 60000;
    detik = int(over / 1000);
    miliDetik = over % 1000;
    lcd.setCursor(1, 1);
    lcd.print(jam, 0);
    lcd.print(" : ");
    lcd.print(menit, 0);
    lcd.print(" : ");
    lcd.print(detik, 0);
    lcd.print(" : ");
    if (jam < 10){
      lcd.print(miliDetik, 0);
      lcd.print(" ");
    }
  }
}