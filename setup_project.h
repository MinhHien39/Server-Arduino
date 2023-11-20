#include <ESP8266WiFi.h>

#include <FirebaseESP8266.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "Realme"
#define WIFI_PASSWORD "0925942425"

#define API_KEY "AIzaSyB5Ui1QeUKRJ6UwzEX618TLFB0LI4kft4I"
#define DATABASE_URL "https://esp8266-a8286-default-rtdb.asia-southeast1.firebasedatabase.app/"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define USER_EMAIL "dinhle29032001@gmail.com"
#define USER_PASSWORD "123123123"

FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;

void setupWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase() {
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  stream.setBSSLBufferSize(2048 /* Rx in bytes, 512 - 16384 */, 512 /* Tx in bytes, 512 - 16384 */);

}