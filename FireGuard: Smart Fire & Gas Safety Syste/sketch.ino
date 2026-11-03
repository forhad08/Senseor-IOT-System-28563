#include "DHT.h"

// পিন ডিফাইন
const int gasSensorPin = 34; // MQ-2 অ্যানালগ পিন
const int dhtPin = 4;        // DHT22 ডাটা পিন
const int servoPin = 13;     // সার্ভো মোটর পিন
const int buzzerPin = 23;    // বাজার পিন
const int ledPin = 22;       // রেড এলইডি পিন

#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

// ESP32 এর নতুন LEDC কনফিগারেশন (Servo PWM এর জন্য)
const int servoChannel = 0;
const int servoFreq = 50;    // 50Hz ফ্রিকোয়েন্সি
const int servoResolution = 16; // 16-bit রেজোলিউশন

int calculatePWM(int degrees) {
  int minPulse = 1638;  // 0 ডিগ্রির জন্য
  int maxPulse = 7864;  // 180 ডিগ্রির জন্য
  return map(degrees, 0, 180, minPulse, maxPulse);
}

const int gasThreshold = 1500; 
const float tempThreshold = 40.0; // ৪০ ডিগ্রি সেলসিয়াসের বেশি হলে বিপদ

void setup() {
  Serial.begin(115200);
  
  pinMode(gasSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  dht.begin();
  
  // ESP32 নতুন ভার্সনের জন্য সঠিক LEDC ফাংশন
  ledcAttachChannel(servoPin, servoFreq, servoResolution, servoChannel);
  
  ledcWrite(servoChannel, calculatePWM(0)); // শুরুতে ভালভ খোলা (0 ডিগ্রি)
  digitalWrite(ledPin, LOW);
  
  Serial.println("Gas & Temperature Safety System Initialized.");
}

void loop() {
  int gasValue = analogRead(gasSensorPin);
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT22 sensor!");
    temperature = 0.0;
  }

  Serial.print("Gas Level: ");
  Serial.print(gasValue);
  Serial.print(" | Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // যদি গ্যাস লিক হয় অথবা তাপমাত্রা অতিরিক্ত বেড়ে যায়
  if (gasValue > gasThreshold || temperature > tempThreshold) {
    digitalWrite(ledPin, HIGH);    
    digitalWrite(buzzerPin, HIGH); 
    ledcWrite(servoChannel, calculatePWM(90)); // ভালভ বন্ধ (90 ডিগ্রি)
    
    Serial.println("WARNING: Hazard Detected! Alarm ON & Valve Closed.");
  } else {
    digitalWrite(ledPin, LOW);     
    digitalWrite(buzzerPin, LOW);  
    ledcWrite(servoChannel, calculatePWM(0)); // ভালভ খোলা (0 ডিগ্রি)
  }
  
  delay(1500);
}
