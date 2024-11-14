#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

int switchState = 0;
int lastSwitchState = 0; // Variabel untuk menyimpan status tombol sebelumnya

int buzzerPin = 9;
int jumpButton = 2;
bool dinoUp = false;  // Status apakah dino sedang melompat
int j;
int score = 0;
unsigned long t; //unsigned long digunakan untuk millis
unsigned long t1;
unsigned long t2;
unsigned long t3;

int objectDelay = 300; // Kecepatan gerakan cactus
int objectDecrease = 20; // Mengurangi delay

byte cactus[8] = { //cactus
  B00101,
  B10101,
  B10101,
  B10101,
  B10110,
  B01100,
  B00100,
  B00100,
};

byte dino[8] = { //dino
  B00000,
  B00111,
  B00101,
  B10111,
  B11100,
  B11111,
  B01101,
  B01100,
};

void setup() {
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.createChar(7, cactus);
  lcd.createChar(6, dino);
  pinMode(buzzerPin, OUTPUT);
  pinMode(jumpButton, INPUT);  // Pastikan tombol sudah diatur sebagai input
  j = 15;
  t = millis();
  t1 = millis();
  t2 = millis();
  t3 = millis();
  Serial.begin(9600);
  
  while (digitalRead(jumpButton) == LOW) {
    lcd.setCursor(0, 0);
    lcd.print("DINO GAME 1.0");
    lcd.setCursor(14, 0);
    lcd.write(6);
    lcd.setCursor(0, 1);
    lcd.print("Press to Start");
    delay(200);
    lcd.clear();
  }
  delay(300);
}

void loop() {
  // Membaca status tombol dengan debouncing
  switchState = digitalRead(jumpButton);

  // Debouncing tombol: hanya deteksi perubahan ketika tombol ditekan atau dilepas
  if (switchState != lastSwitchState) {
    delay(400); // Tunggu sedikit untuk memastikan tombol stabil
    if (switchState == LOW) { // Tombol ditekan
      dinoUp = true;  // DINO JUMP
    } else { // Tombol dilepas
      dinoUp = false;  // Kembali ke posisi semula
    }
  }
  lastSwitchState = switchState;  // Simpan status tombol terakhir

  if ((millis() - t) >= objectDelay) { // Delay gerakan cactus
    t = millis();
    lcd.clear();
    lcd.setCursor(j, 1);
    lcd.write(7);
    j = j - 1;
    if (j == -1) {
      j = 15;
    }
    lcd.setCursor(6, 0);
    lcd.print("Score:");
    lcd.print(score);
  }

  if ((millis() - t1) >= 50) {
    t1 = millis();
    
    // Hapus dino dari posisi sebelumnya sebelum menampilkannya di posisi baru
    lcd.setCursor(2, 0);
    lcd.print(" ");  // Hapus dino dari baris 0
    lcd.setCursor(2, 1);
    lcd.print(" ");  // Hapus dino dari baris 1

    if (dinoUp == true) {
      // DINO JUMP (tetap di baris 0 selama tombol ditekan)
      lcd.setCursor(2, 0);
      lcd.write(6);  // Tampilkan dino di posisi atas (baris 0)
    } else {
      // Tampilkan dino di posisi semula (baris 1) saat tombol dilepas
      lcd.setCursor(2, 1);
      lcd.write(6);  // Tampilkan dino di posisi bawah (baris 1)
    }
  }

  if ((millis() - t2) >= 1500 && dinoUp == true) {
    t2 = millis();
    // Biarkan dino tetap di posisi atas selama tombol ditekan
  }

  if (((millis() - t3) >= 1000) && (j == 1)) {
    t3 = millis();
    if (dinoUp == true) {
      tone(buzzerPin, 700, 100);
      score = score + 1;
      if (score % 5 == 0) {
        objectDelay = objectDelay - objectDecrease;
        if (objectDelay <= 100) { // Kecepatan maksimum
          objectDecrease = 0; // Menjaga kecepatan objek tetap maksimal
        }
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      tone(buzzerPin, 700, 100);
      lcd.print("   GAME OVER!   ");
      lcd.setCursor(0, 1);
      lcd.print("Push Button");
      score = 0;
      j = 15;
      objectDelay = 300; // Mengembalikan kecepatan ke nilai semula
      while (digitalRead(jumpButton) == LOW) {
        // Menunggu tombol ditekan untuk memulai ulang
      }
      delay(500);
    }
  }
}
