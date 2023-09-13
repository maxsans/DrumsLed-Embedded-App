#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;
bool deviceConnected = false;
bool deviceDiscoverable = true;
int i = 0;
String receivedMessage = "";
bool messageDisplayed = false;
std::string receivedData = "";
const std::string JSON_DELIMITER = "<!JSON_END!>"; 



class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      deviceDiscoverable = false;
      pAdvertising->stop(); // Stop advertising when connected
      receivedData = "";

      Serial.println("Téléphone connecté !");
    }

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      deviceDiscoverable = true;
      pAdvertising->start(); // Start advertising when disconnected
      receivedData = "";
      Serial.println("Téléphone déconnecté. Le BLE est à nouveau discoverable.");
    }

};

class MyCallbacks : public BLECharacteristicCallbacks {
void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    receivedData += value;

    size_t found = receivedData.find(JSON_DELIMITER);

    while (found != std::string::npos)
    {
      String jsonStr = receivedData.substr(0, found).c_str();
      Serial.println("Received JSON:");
      Serial.println(jsonStr);

      receivedData = receivedData.substr(found + JSON_DELIMITER.length());
      found = receivedData.find(JSON_DELIMITER);
    }
  }
};

void setup() {
  Serial.begin(9600);

  BLEDevice::init("ESP32_BLE_Example");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                    CHARACTERISTIC_UUID,
                    BLECharacteristic::PROPERTY_READ |
                    BLECharacteristic::PROPERTY_WRITE |
                    BLECharacteristic::PROPERTY_NOTIFY
                  );
  pCharacteristic->setCallbacks(new MyCallbacks());


  pCharacteristic->setValue("Initial Value");

  pService->start();

  pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);  // not specified
  pAdvertising->setAppearance(0x0008);// Generic Phone" appearance
  pAdvertising->start();

  Serial.println("Le BLE est discoverable. Attente de la connexion...");
}

void loop() {
  if (deviceConnected) {
    i++;

    // delay(1000); 
    // String i_str = String(i);
    // String str = "New data"  + i_str;
    // pCharacteristic->setValue(str.c_str());
    // pCharacteristic->notify();
    // std::string value = pCharacteristic->getValue();
    // if (value.length() > 0) {
    //     receivedMessage = String(value.c_str());
    //     pCharacteristic->setValue("");
    //     Serial.println("Message reçu : " + receivedMessage);
    //     receivedMessage = "";
    // }
  }

  if (deviceDiscoverable && !deviceConnected) {
    Serial.println("Le BLE est discoverable. Attente de la connexion...");
    delay(2000); // Attente de 2 secondes entre les tentatives de découverte
  }
}
