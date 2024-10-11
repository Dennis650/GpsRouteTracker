#include <Arduino.h>
#include <TrackerConfig.h>
#include <GpsData.h>
#include <global_vars.h>
#include <time_functions.h>
#include <sd_functions.h>
#include <web_functions.h>
#include <screen_functions.h>
#include <temperature_functions.h>
#include <TrackerGps.h>
#include <Distances.h>
#include <TrackerSD.h>
//#include <ble.h> 
#include <ScreenBoot.h>
//#include <ANCS_functions.h>

void setup()
{
    Serial.begin(115200);
    while(!Serial) {
        delay(100);
    }

    trackerGps.begin();
    getTrackerSD()->begin();

    loadTrackerConfig();
    rotateOled();
    screenSetup();

    temperatureSetup();
    
    loadLapTimerData();

    distances.init();
    loadDistances();

    wifiSetup();
 
    //setupBle();
 }

void testmode() {
    distances.addDistance(50);
    
    gpsData.course = gpsData.course + 1;
    if (gpsData.course>359) {
        gpsData.course = 0;
    }
    gpsData.courseToDestination = 180;

    /*
    Serial.println(getTemperatureSensorValue()); //Dauert scheinbar lange, nur alle 30 Sekunden lesen
    Serial.print(".");
    trackerGps.gpsPrintInfos();
    */
}

void loop()
{
    temperatureLoop();
    trackerGps.gpsFetchDataAndCalcDistances();
    trackPostionLoop();
    saveDistancesLoop();
    screenLoop();
    handleWebClient();
    handleArduinoOTA();
    loopWifiConnect();
    //loopBle();
    
    trackerGps.readSerialGPS();

    //testmode();
}