#include "DHT.h"

// Define pins
const int DHTPIN = 4;    // DHT22 data pin
const int ledPin = 23;   // LED pin

// Define sensor type
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();
  pinMode(ledPin, OUTPUT);
}

void loop() {
  delay(2000); // Wait 2 seconds between measurements

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Check if sensor readings are valid
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Turn LED on if temperature exceeds 30°C
  if (t > 30.0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}