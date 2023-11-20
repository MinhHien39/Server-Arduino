#include <FirebaseESP8266.h>

#include "UUID.h"

UUID uuid;

FirebaseData fbdo;

void addDHT(String houseId, String floorId, String roomId) {
  uuid.generate();
  String dhtId = String(uuid.toCharArray());

  FirebaseJson json;
  json.add("temperature", 27.5);
  json.add("humidity", 70.3);

  Serial.printf("Add house... %s\n\n", Firebase.setJSON(fbdo, ("/test1/" + houseId + "/floors/" + floorId + "/rooms/" + roomId + "/DHT/" + dhtId).c_str(), json) ? "ok" : fbdo.errorReason().c_str());
}

void addDevice(String houseId, String floorId, String roomId, String name) {
  uuid.generate();
  String deviceId = String(uuid.toCharArray());

  FirebaseJson json;
  json.add("name", name);
  json.add("startTime", 0);
  json.add("endTime", 0);
  json.add("state", 0);

  Serial.printf("Add house... %s\n\n", Firebase.setJSON(fbdo, ("/test1/" + houseId + "/floors/" + floorId + "/rooms/" + roomId + "/devices/" + deviceId).c_str(), json) ? "ok" : fbdo.errorReason().c_str());
}

void addOtherSensor(String houseId, String floorId, String roomId, String name) {
  uuid.generate();
  String otherSensorId = String(uuid.toCharArray());

  FirebaseJson json;
  json.add("name", name);
  json.add("state", 0);

  Serial.printf("Add house... %s\n\n", Firebase.setJSON(fbdo, ("/test1/" + houseId + "/floors/" + floorId + "/rooms/" + roomId + "/otherSensors/" + otherSensorId).c_str(), json) ? "ok" : fbdo.errorReason().c_str());
}

void addDevices(String houseId, String floorId, String roomId, String devices[], int numOfDevices) {
  for (int i = 0; i < numOfDevices; i++) {
    if (devices[i].indexOf("Light") != -1 || devices[i].indexOf("Fan") != -1) {
      addDevice(houseId, floorId, roomId, devices[i]);
    } else if (devices[i].indexOf("Gas") != -1 || devices[i].indexOf("Rain") != -1) {
      addOtherSensor(houseId, floorId, roomId, devices[i]);
    } else {
      addDHT(houseId, floorId, roomId);
    }
  }
}

void addRoom(String houseId, String floorId, String name) {
  uuid.generate();
  String roomId = String(uuid.toCharArray());

  FirebaseJson json;
  json.add("name", name);

  Serial.printf("Add house... %s\n\n", Firebase.setJSON(fbdo, ("/test1/" + houseId + "/floors/" + floorId + "/rooms/" + roomId).c_str(), json) ? "ok" : fbdo.errorReason().c_str());
  if (name.indexOf("Kitchen") != -1) {
    String devices[] = {"Light 1", "Light 2", "Light 3", "Fan", "DHT", "Gas sensor"};
    int numOfDevices = sizeof(devices) / sizeof(devices[0]);
    addDevices(houseId, floorId, roomId, devices, numOfDevices);
  } else if (name.indexOf("Room") != -1) {
    String devices[] = {"Light 1", "Light 2", "Fan"};
    int numOfDevices = sizeof(devices) / sizeof(devices[0]);
    addDevices(houseId, floorId, roomId, devices, numOfDevices);
  } else if (name.indexOf("Yard") != -1) {
    String devices[] = {"Light 1", "Light 2", "Rain sensor"};
    int numOfDevices = sizeof(devices) / sizeof(devices[0]);
    addDevices(houseId, floorId, roomId, devices, numOfDevices);
  } else if (name.indexOf("Bath") != -1) {
    String devices[] = {"Light 1", "Light 2"};
    int numOfDevices = sizeof(devices) / sizeof(devices[0]);
    addDevices(houseId, floorId, roomId, devices, numOfDevices);
  } else {
    String devices[] = {"Light 1", "Light 2", "Fan", "DHT"};
    int numOfDevices = sizeof(devices) / sizeof(devices[0]);
    addDevices(houseId, floorId, roomId, devices, numOfDevices);
  }
}

void addFloor(String houseId, String name, String rooms[], int numOfRooms) {
  uuid.generate();
  String floorId = String(uuid.toCharArray());

  FirebaseJson json;
  json.add("name", name);

  Serial.printf("Add house... %s\n\n", Firebase.setJSON(fbdo, ("/test1/" + houseId + "/floors/" + floorId).c_str(), json) ? "ok" : fbdo.errorReason().c_str());
  for (int i = 0; i < numOfRooms; i++) {
    addRoom(houseId, floorId, rooms[i]);
  }
}

void addHouse(String name, String emailHost, String telHost, String houseKey) {
  uuid.generate();
  String houseId = String(uuid.toCharArray());

  FirebaseJson json;
  json.add("name", name);
  json.add("emailHost", emailHost);
  json.add("telHost", telHost);
  json.add("houseKey", houseKey);

  Serial.printf("Add house... %s\n\n", Firebase.setJSON(fbdo, ("/test1/" + houseId + "/").c_str(), json) ? "ok" : fbdo.errorReason().c_str());


  // Add floors
  String rooms1[] = { "Yard", "Living room", "Kitchen room", "Room 1", "Room 2", "Bathroom" };
  int numOfFloors1 = sizeof(rooms1) / sizeof(rooms1[0]);
  addFloor(houseId, "Floor 1", rooms1, numOfFloors1);
  String rooms2[] = { "Room 1", "Room 2", "Bathroom" };
  int numOfFloors2 = sizeof(rooms2) / sizeof(rooms2[0]);
  addFloor(houseId, "Floor 2", rooms2, numOfFloors2);
}
