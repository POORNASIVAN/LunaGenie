#include <FirebaseESP32.h>


#define FIREBASE_HOST "your-firebase-database.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-auth-token"
FirebaseData firebaseData;

#define VIBRATION_PIN 5
#define POT_PIN 34

void setup() {
  Serial.begin(115200);


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // Initialize Vibration Motor Pin
  pinMode(VIBRATION_PIN, OUTPUT);
  digitalWrite(VIBRATION_PIN, LOW);

  Serial.println("Setup complete.");
}

void loop() {
  if (Firebase.getFloat(firebaseData, "/HeartRate")) {
    float heartRate = firebaseData.floatData();

    // Adjust motor based on heart rate
    if (heartRate < 60) {
      digitalWrite(VIBRATION_PIN, HIGH);
    } else {
      digitalWrite(VIBRATION_PIN, LOW);
    }

    int potValue = analogRead(POT_PIN);
    float motorVoltage = map(potValue, 0, 4095, 0, 3.3);
    analogWrite(VIBRATION_PIN, motorVoltage);

    Serial.print("Heart Rate: ");
    Serial.println(heartRate);
    Serial.print("Vibration Motor Voltage: ");
    Serial.println(motorVoltage);
  }

  delay(1000);
}
