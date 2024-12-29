#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <FirebaseESP32.h>
#include "RTClib.h"
#include "MAX30100_PulseOximeter.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);


#define FIREBASE_HOST "your-firebase-database.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-auth-token"
FirebaseData firebaseData;


RTC_DS3231 rtc;


PulseOximeter pox;
#define REPORTING_PERIOD_MS 1000
uint32_t tsLastReport = 0;

void onBeatDetected() {
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize OLED
  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    for (;;);
  }

 
  if (!pox.begin()) {
    Serial.println("Failed to initialize MAX30100");
    for (;;);
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);

 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Serial.println("Setup complete.");
}

void loop() {
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    tsLastReport = millis();

    float heartRate = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    // Display data on OLED
    DateTime now = rtc.now();
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Time: ");
    display.print(now.hour());
    display.print(":");
    display.print(now.minute());
    display.print(":");
    display.println(now.second());
    display.print("Heart Rate: ");
    display.println(heartRate);
    display.print("SpO2: ");
    display.println(spo2);
    display.display();

  
    Firebase.setFloat(firebaseData, "/HeartRate", heartRate);
    Firebase.setFloat(firebaseData, "/SpO2", spo2);
  }
}
