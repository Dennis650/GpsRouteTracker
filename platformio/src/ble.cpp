#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <time_functions.h>
#include <TrackerGps.h>
#include <ArduinoJson.h>
#include <TimeLib.h>  
#include <TrackerConfig.h>
#include <GpsData.h>
#include <global_vars.h>
#include <sd_functions.h>
#include <TrackerConfig.h>
#include <GpsData.h>

static unsigned long lastBleUpdate = 0;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicGpsData = NULL;
BLECharacteristic* pCharacteristicConfigData = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID "f5596d9c-c1fb-11ec-9d64-0242ac120003"
#define CHARACTERISTIC_GPSDATA_UUID "08aa70bc-c1fc-11ec-9d64-0242ac120003"
#define CHARACTERISTIC_CONFIGDATA_UUID "278127ce-c1fc-11ec-9d64-0242ac120003"

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected!");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected!");
    }
};

class CharacteristicCallBack : public BLECharacteristicCallbacks {
public:
  //This method not called
  void onWrite(BLECharacteristic *characteristic_) override
  {
    Serial.println("onWrite");

    std::string value = characteristic_->getValue();

    Serial.print("new Value: ");
    Serial.println(value.c_str());

    const char* json = value.c_str();
    Serial.println(json);

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.println(F("Failed to read file, using default values"));
        Serial.println(error.code());
        Serial.println(error.c_str());
    } else {
        Serial.println("OK");
        trackerConfig.home_latitude = doc["home_latitude"];
        trackerConfig.home_longitude = doc["home_longitude"];
        Serial.println(trackerConfig.home_latitude,6);
        Serial.println(trackerConfig.home_longitude,6);
        trackerConfig.destination_latitude = doc["destination_latitude"];
        trackerConfig.destination_longitude = doc["destination_longitude"];
        Serial.println(trackerConfig.destination_latitude,6);
        Serial.println(trackerConfig.destination_longitude,6);

        saveTrackerConfig();
    }
  }
};

void setupBle() {    

    // Create the BLE Device
    BLEDevice::init("GpsRouteTracker");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristicGpsData = pService->createCharacteristic(
                        CHARACTERISTIC_GPSDATA_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                        );
  
    pCharacteristicConfigData = pService->createCharacteristic(
                    CHARACTERISTIC_CONFIGDATA_UUID,
                    BLECharacteristic::PROPERTY_READ   |
                    BLECharacteristic::PROPERTY_WRITE  |
                    BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_INDICATE
                    );

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristicGpsData->addDescriptor(new BLE2902());
    pCharacteristicConfigData->addDescriptor(new BLE2902());

    pCharacteristicConfigData->setCallbacks(new CharacteristicCallBack());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}


void updateGpsData(GpsData &gpsData) {
    DynamicJsonDocument jsonDocGpsData(256);
    jsonDocGpsData["satellites"] = gpsData.satellites;
    jsonDocGpsData["lat"] = gpsData.latitude;
    jsonDocGpsData["lon"] = gpsData.longitude;
    jsonDocGpsData["alt"] = gpsData.altitude;
    jsonDocGpsData["speed"] = gpsData.speed;
    //gpsdata["course"] = gpsData.course;
    //gpsdata["distanceMetersSinceLastFix"] = gpsData.distanceMetersSinceLastFix;
    //gpsdata["stopped"] = gpsData.stopped;
    jsonDocGpsData["wiringError"] = gpsData.wiringError;
    
    char date[32];
    sprintf(date, "%02d.%02d.%02d ", day(), month(), year());
    char time[32];
    sprintf(time, "%02d:%02d:%02d ", hour(), minute(), second());
    jsonDocGpsData["date"] = date;
    jsonDocGpsData["time"] = time;

    char jsonOutGpsData[256];
    serializeJson(jsonDocGpsData, jsonOutGpsData);

    pCharacteristicGpsData->setValue(jsonOutGpsData);
    pCharacteristicGpsData->notify();
}

void updateConfigData(TrackerConfig &trackerConfig) {
    DynamicJsonDocument jsonDocConfigData(256);
    jsonDocConfigData["home_latitude"] = trackerConfig.home_latitude;
    jsonDocConfigData["home_longitude"] = trackerConfig.home_longitude;
    jsonDocConfigData["destination_latitude"] = trackerConfig.destination_latitude;
    jsonDocConfigData["destination_longitude"] = trackerConfig.destination_longitude;

    char jsonOutConfigData[256];
    serializeJson(jsonDocConfigData, jsonOutConfigData);

    pCharacteristicConfigData->setValue(jsonOutConfigData);
    pCharacteristicConfigData->notify();
}

void loopBle() {
    // notify changed value
    if (deviceConnected) {

        unsigned long currentMillis = millis();

        if (currentMillis - lastBleUpdate >= 1000)
        {
            lastBleUpdate = currentMillis;
            
            updateGpsData(gpsData);
            updateConfigData(trackerConfig);
        
            delay(100); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
        }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
