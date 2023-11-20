#include <Arduino.h>
#include "DHT.h"
#include "setup_project.h"
#include "write_data.h"
#include "listen_data_change.h"

#define GAS_SENSOR_PIN A0
#define GAS_SENSOR_THRESHOLD 500

#define DHTPIN 2  // D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;

int count = 0;

int pinNumber[] = { 16, 5, 4 };

void setup() {
  Serial.begin(115200);

  setupWifi();

  setupFirebase();
  // addHouse("Nhà của Hiền", "tranminhhien@gmail.com", "0922942242", "hien@242");
  // addHouse("Nhà của Hường", "lethihuong@gmail.com", "0984842341", "huong@341");

  listenChange();

  dht.begin();
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    sendDataPrevMillis = millis();
    count++;
    FirebaseJson dhtJson;
    dhtJson.add("temperature", t);
    dhtJson.add("humidity", h);

    FirebaseJson gasJson;
    int sensorValue = analogRead(GAS_SENSOR_PIN);
    gasJson.add("name", "Gas sensor");
    gasJson.add("state", sensorValue);

    Serial.printf("Set dhtJson... %s\n\n", Firebase.setJSON(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/DHT/1e1397cf-de32-4c6b-bc51-52b5ec8277ba/", dhtJson) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set gasJson... %s\n\n", Firebase.setJSON(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/otherSensors/a5a13596-553e-4eed-8cf4-f676036f820d/", gasJson) ? "ok" : fbdo.errorReason().c_str());

    if (Firebase.getFloat(fbdo, "/test1/e3ba4698-d64b-447e-81f5-0bf0e09700eb/floors/e72cf36f-f9c5-4dee-b11a-951c0e3dc638/rooms/4d71d15e-9dc8-4e39-b506-facef152130a/DHT/1e1397cf-de32-4c6b-bc51-52b5ec8277ba/temperature")) {
      float temp = fbdo.floatData();  // Get the float value
      if (temp > 35 || sensorValue > 500) {
        tone(D5, 1000);
        Serial.println("> 35");
      } else {
        noTone(D5);
        Serial.println("< 35");
      }
    } else {
      Serial.println("Failed to get temperature from Firebase");
    }
  }



  if (dataChanged) {
    dataChanged = false;
    // When stream data is available, do anything here...
  }

  // After calling stream.keepAlive, now we can track the server connecting status
  if (!stream.httpConnected()) {
    // Server was disconnected!
  }
}
