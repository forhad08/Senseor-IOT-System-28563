#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C এড্রেস (সাধারণত 0x27 বা 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 9;
const int echoPin = 8;
const int pumpPin = 7;

// ট্যাংকের উচ্চতা ও দূরত্ব কনফিগারেশন (সেন্টিমিটারে)
const int maxDistance = 200; // ট্যাংক সম্পূর্ণ খালি থাকলে দূরত্ব
const int minDistance = 20;  // ট্যাংক সম্পূর্ণ ভর্তি থাকলে দূরত্ব

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  
  Serial.begin(9600);
  lcd.init();                      
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("TankMate System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Ultrasonic সেন্সর ট্রিগার পাল্স পাঠানো
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // ইকো পিন থেকে সময় পরিমাপ
  long duration = pulseIn(echoPin, HIGH);
  
  // দূরত্ব হিসাব (সেন্টিমিটারে)
  int distance = duration * 0.034 / 2;
  
  // পানির লেভেল পার্সেন্টেজে রূপান্তর (0% থেকে 100%)
  int waterLevel = map(distance, maxDistance, minDistance, 0, 100);
  waterLevel = constrain(waterLevel, 0, 100);
  
  // অটোমেটিক পাম্প কন্ট্রোল লজিক
  static bool pumpStatus = false;
  if (waterLevel <= 20) {
    pumpStatus = true;  // পানি ২০% বা তার নিচে নামলে পাম্প অন হবে
  } else if (waterLevel >= 90) {
    pumpStatus = false; // পানি ৯০% বা তার বেশি হলে পাম্প অফ হবে
  }
  
  digitalWrite(pumpPin, pumpStatus ? HIGH : LOW);

  // LCD ডিসপ্লেতে আপডেট দেখানো
  lcd.setCursor(0, 0);
  lcd.print("Water Level: ");
  if (waterLevel < 100) lcd.print(" ");
  if (waterLevel < 10) lcd.print(" ");
  lcd.print(waterLevel);
  lcd.print("%  ");
  
  lcd.setCursor(0, 1);
  lcd.print("Pump Status: ");
  lcd.print(pumpStatus ? "ON " : "OFF");
  
  delay(1000);
}
