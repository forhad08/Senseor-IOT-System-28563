#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ESP32 এর I2C পিন (GPIO 21 = SDA, GPIO 22 = SCL)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int moisturePin = 34; // ESP32 এর ADC পিন (GPIO 34)
const int stepPin = 18;    // A4988 Step Pin
const int dirPin = 19;     // A4988 Direction Pin

// ESP32-এ অ্যানালগ রিডিং সাধারণত 0 থেকে 4095 পর্যন্ত হয় (12-bit ADC)
const int threshold = 2000;  

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(moisturePin, INPUT);
  
  // LCD ইনিশিয়ালাইজেশন
  Wire.begin(21, 22); // SDA, SCL explicit define
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("ESP32 Smart Plant");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  // ESP32-এর অ্যানালগ ইনপুট রিড করা (0 - 4095)
  int moistureValue = analogRead(moisturePin); 
  
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moistureValue);
  lcd.print("   "); // আগের মান মুছে ফেলার জন্য অতিরিক্ত স্পেস

  // মান থ্রেশহোল্ডের নিচে নামলে মোটর ঘুরবে
  if (moistureValue < threshold) {
    lcd.setCursor(0, 1);
    lcd.print("Status: Watering! ");
    
    // Stepper Motor ঘোরানোর সিগন্যাল
    digitalWrite(dirPin, HIGH);
    for(int i = 0; i < 50; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Status: Soil Wet  ");
  }
  
  delay(500);
}