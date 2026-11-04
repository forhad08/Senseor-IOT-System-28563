// পিন ডিফাইন
const int trigPin = 5;
const int echoPin = 18;
const int buzzerPin = 23;
const int redLedPin = 22; // সিঙ্গেল রেড LED পিন

void setup() {
  Serial.begin(115200);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  
  Serial.println("Smart Ultrasonic Parking Assistant Initialized.");
}

void loop() {
  // আল্ট্রাসনিক সেন্সর ট্রিগার পালস পাঠানো
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // ইকো পিন থেকে সময় মাপা
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // সেন্টিমিটারে রূপান্তর

  // শুরুর দিকের ফলস বা জিরো (0) মান ফিল্টার করার জন্য কন্ডিশন
  if (distance > 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // পার্কিং দূরত্ব অনুযায়ী LED এবং Buzzer কন্ট্রোল
    if (distance <= 5) { 
      // একদম কাছাকাছি বা দেওয়ালে লেগে যাওয়ার অবস্থা (জরুরি বিপদ - একটানা লাল আলো ও বাজার)
      digitalWrite(redLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH); 
    }
    else if (distance > 5 && distance <= 30) { 
      // খুব কাছে (বিপদ - লাল LED এবং একটানা বাজার অন থাকবে)
      digitalWrite(redLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH); 
    } 
    else if (distance > 30 && distance <= 80) { 
      // মাঝারি দূরত্ব (সতর্কতা - লাল LED এবং বাজার বিরতিতে জ্বলবে/বাজবে)
      digitalWrite(redLedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(redLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
    } 
    else { 
      // নিরাপদ দূরত্ব (> ৮০ সেমি - LED এবং বাজার সম্পূর্ণ অফ থাকবে)
      digitalWrite(redLedPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }
  }
  
  delay(400);
}
