#include <Arduino.h>
#include <SdFat.h>
#include <ArduinoJson.h>

#include <bike_config.h>
#include <bike_secure.h>
#include <bike_coordinates.h>
#include <Distances.h>
#include <TrackerConfig.h>
#include <GpsData.h>
#include <global_vars.h>
#include <time_functions.h>
#include <TimeLib.h>
#include <TrackerSD.h>
#include <screen_functions.h>

//Tracking
static unsigned long lastTrackEntry = 0;

const char *distancesFilename = "distance.dat";
static int distancesSaveInterval = 30000;
static unsigned long lastDistancesIntervalSave = 0;
static unsigned long lastDistancesStoppedSave = 0;

//Config
const char *configFilename = "tracker.conf";


//LapTime
const char *lapTimerFilename = "laptimer.dat";


// Loads the Distances from a file
void loadDistances()
{
    File32 file = getTrackerSD()->getSd().open(distancesFilename);

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        char messageRow1[20] = "Failed to read file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);
        Serial.println(F("Failed to read file Distances"));
    } else {
        distances.setOdo(doc["distanceODO"]);
        distances.setTrip1(doc["distanceTrip1"]);
        distances.setTrip1StartMillis(doc["startMillisTrip1"]);
        distances.setTrip2(doc["distanceTrip2"]);
        distances.setTrip2StartMillis(doc["startMillisTrip2"]);
        distances.setFuel(doc["distanceFuel"]);
    }

    file.close();
}

void saveDistances()
{
    getTrackerSD()->begin();

    // Delete existing file, otherwise the configuration is appended to the file
    getTrackerSD()->getSd().remove(distancesFilename);

    // Open file for writing
    File32 file = getTrackerSD()->getSd().open(distancesFilename, FILE_WRITE);
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        char messageRow1[22] = "Failed to create file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Set the values in the document
    doc["distanceODO"] = distances.getOdo();
    doc["distanceTrip1"] = distances.getTrip1();
    doc["startMillisTrip1"] = distances.getTrip1StartMillis();
    doc["distanceTrip2"] = distances.getTrip2();
    doc["startMillisTrip2"] = distances.getTrip2StartMillis();
    doc["distanceFuel"] = distances.getFuel();

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0)
    {
        char messageRow1[24] = "Failed to write to file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);
        
        Serial.println(F("Failed to write to file"));
    }
    else
    {
        Serial.println(F("Distances erfolgreich geschrieben!"));
    }

    // Close the file
    file.flush();
    file.close();
}

void saveTrackerConfig()
{
    getTrackerSD()->begin();

    // Delete existing file, otherwise the configuration is appended to the file
    getTrackerSD()->getSd().remove(configFilename);

    // Open file for writing
    File32 file = getTrackerSD()->getSd().open(configFilename, FILE_WRITE);
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        
        char messageRow1[22] = "Failed to create file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);
        
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Set the values in the document
    doc["tankinhalt"] = trackerConfig.tankinhalt;
    doc["tankreserve"] = trackerConfig.tankreserve;
    doc["durchschnittsverbrauch"] = trackerConfig.durchschnittsverbrauch;
    doc["wlanMode"] = trackerConfig.wlanMode;
    doc["wlanApSsid"] = trackerConfig.wlanApSsid;
    doc["wlanApPassword"] = trackerConfig.wlanApPassword;
    doc["wlanClientSsid"] = trackerConfig.wlanClientSsid;
    doc["wlanClientPassword"] = trackerConfig.wlanClientPassword;
    
    doc["device"] = trackerConfig.device;

    doc["displayRotate"] = trackerConfig.displayRotate;

    doc["home_latidude"] = trackerConfig.home_latitude;
    doc["home_longitude"] = trackerConfig.home_longitude;

    doc["destination_latidude"] = trackerConfig.destination_latitude;
    doc["destination_longitude"] = trackerConfig.destination_longitude;
    
    // Serialize JSON to file
    if (serializeJson(doc, file) == 0)
    {
        Serial.println(F("Failed to write to file"));
        
        char messageRow1[24] = "Failed to write to file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);
        
    }
    else
    {
        Serial.println(F("TrackerConfig erfolgreich geschrieben!"));
    }

    // Close the file
    file.flush();
    file.close();
}

void deleteTackerConfig()
{
    getTrackerSD()->begin();
    getTrackerSD()->getSd().remove(configFilename);
}

void initTrackerConfig() {
    Serial.println(F("initTrackerConfig"));

    trackerConfig.tankinhalt= BIKE_TANKINHALT;
    trackerConfig.tankreserve = BIKE_TANKRESERVE;
    trackerConfig.durchschnittsverbrauch = BIKE_DURCHSCHNITTSVERBRAUCH;
    strlcpy(trackerConfig.wlanMode, "AP", sizeof(trackerConfig.wlanMode));     //AP, CLIENT, OFF

    strlcpy(trackerConfig.wlanApSsid, BIKE_NAME, sizeof(trackerConfig.wlanApSsid));     
    strlcpy(trackerConfig.wlanApPassword, BIKE_SECRET, sizeof(trackerConfig.wlanApPassword));   
    strlcpy(trackerConfig.wlanClientSsid, WLAN_CLIENT_SSID, sizeof(trackerConfig.wlanClientSsid));     
    strlcpy(trackerConfig.wlanClientPassword, WLAN_CLIENT_PASSWORD, sizeof(trackerConfig.wlanClientPassword));   

    strlcpy(trackerConfig.device, BIKE_NAME, sizeof(trackerConfig.device));  

    trackerConfig.displayRotate = BIKE_DISPLAY_ROTATE;

    trackerConfig.home_latitude = HOME_LATIDUDE;
    trackerConfig.home_longitude = HOME_LONGITUDE;

    trackerConfig.destination_latitude = DESTINATION_LATIDUDE;
    trackerConfig.destination_longitude = DESTINATION_LONGITUDE;
}

// Loads the configuration from a file
void loadTrackerConfig()
{
    Serial.println("loadTrackerConfig");
    File32 file = getTrackerSD()->getSd().open(configFilename);

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<1024> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        Serial.println(F("Failed to read file, using default configuration"));
        
        initTrackerConfig();

        Serial.println(F("Save new TrackerConfig using default configuration!"));

        saveTrackerConfig();
    } else {
        Serial.println("Json in trackConfig überführen...");
        trackerConfig.tankinhalt= doc["tankinhalt"];
        trackerConfig.tankreserve = doc["tankreserve"];
        trackerConfig.durchschnittsverbrauch = doc["durchschnittsverbrauch"];

        Serial.println("Json nach Tank...");

        strlcpy(trackerConfig.wlanMode, doc["wlanMode"],  sizeof(trackerConfig.wlanMode));  
        strlcpy(trackerConfig.wlanApSsid, doc["wlanApSsid"], sizeof(trackerConfig.wlanApSsid));     
        strlcpy(trackerConfig.wlanApPassword, doc["wlanApPassword"], sizeof(trackerConfig.wlanApPassword));   
        strlcpy(trackerConfig.wlanClientSsid, doc["wlanClientSsid"], sizeof(trackerConfig.wlanClientSsid));     
        strlcpy(trackerConfig.wlanClientPassword, doc["wlanClientPassword"], sizeof(trackerConfig.wlanClientPassword));   

        Serial.println("Json nach WLAN...");

        strlcpy(trackerConfig.device, doc["device"], sizeof(trackerConfig.device));   

        trackerConfig.displayRotate= doc["displayRotate"];

        trackerConfig.home_latitude = doc["home_latidude"];
        trackerConfig.home_longitude = doc["home_longitude"];

        trackerConfig.destination_latitude = doc["destination_latidude"];
        trackerConfig.destination_longitude = doc["destination_longitude"];
    }
    file.close();
}

void trackPostionLoop()
{
    bool trackPointNow = false; 

    //FürDebug alle 5 Sekunden
    //trackPointNow = (millis() - lastTrackEntry) > 5 * 1000;
     
    if (gpsData.satellites > 1 && gpsData.location_valid) {
        unsigned long millisSinceLastTrackEntry = millis() - lastTrackEntry;

        if (gpsData.speed == 0) {
            trackPointNow = false;
        } else if (gpsData.speed <= 50) {
            trackPointNow = millisSinceLastTrackEntry > 5 * 1000;
        } else if (gpsData.speed <= 75) {
            trackPointNow = millisSinceLastTrackEntry > 15 * 1000;
        } else if (gpsData.speed <= 100) {
            trackPointNow = millisSinceLastTrackEntry > 30 * 1000;
        } else {
            trackPointNow = millisSinceLastTrackEntry > 60 * 1000;
        }
    }

    if (trackPointNow)
    {
        Serial.println("!!!Track Position!!!");

        getTrackerSD()->begin();

        lastTrackEntry = millis();

        char filename[14];
        sprintf(filename, "%02d%02d%02dtr.gpx", (year()-2000), month(), day());

        bool fileExists = getTrackerSD()->getSd().exists(filename);
        File32 file = getTrackerSD()->getSd().open(filename, FILE_WRITE);

        if (!file)
        {
            Serial.println("Failed to read file GPX");
            
            char messageRow1[20] = "Failed to read file";
            char messageRow2[20] = "";
            showMessageScreen(messageRow1, messageRow2);
            
            getTrackerSD()->getSd().errorPrint(&Serial);
            Serial.println(getTrackerSD()->getSd().sdErrorCode());
            Serial.println(getTrackerSD()->getSd().sdErrorData());
        }
        else
        {   
            if (!fileExists)
            {
                //TODO: bei createor ein DeviceName (Dr650 / AfricaTwin) einfügen
                file.print(
                    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                    "<gpx version=\"1.1\" creator=\"GpsRouteTracker\">\r\n"
                    "<trk>\r\n<trkseg>\r\n"); //heading of gpx file
                file.print("</trkseg>\r\n</trk>\r\n</gpx>\r\n");
            }

            unsigned long filesize = file.size();
            // back up the file pointer to just before the closing tags
            filesize -= 27;

            file.seek(filesize);
            file.print(F("<trkpt lat=\""));
            file.print(gpsData.latitude, 7);
            file.print(F("\" lon=\""));
            file.print(gpsData.longitude, 7);
            file.println(F("\">"));
            file.print("<name>");
            char time[28];
            sprintf(time, "%02d.%02d.%4d-%02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());
            file.print(time);
            file.println("</name>");
            file.print(F("<time>"));

            if (isSummertime())
            {
                sprintf(time, "%4d-%02d-%02dT%02d:%02d:%02d+02:00", year(), month(), day(), hour(), minute(), second());
            }
            else
            {
                sprintf(time, "%4d-%02d-%02dT%02d:%02d:%02d+01:00", year(), month(), day(), hour(), minute(), second());
            }

            file.print(time);
            file.println(F("</time>"));
            file.print(F("<ele>"));
            file.print(gpsData.altitude, 1);
            file.print(F("</ele>\r\n<hdop>"));
            file.print(gpsData.hdpo, 2);
            file.print(F("</hdop>\r\n<course>"));
            file.print(gpsData.course, 1);
            file.print(F("</course>\r\n<speed>"));
            file.print(gpsData.speed, 1);
            file.print(F("</speed>\r\n<sat>"));
            file.print(gpsData.satellites);
            file.println(F("</sat>\r\n</trkpt>"));
            file.print(F("</trkseg>\r\n</trk>\r\n</gpx>\r\n"));
            file.close();

            file.flush();
            file.close();
        }
    }
}

void trackWaypoint()
{
    getTrackerSD()->begin();

    char filename[14];
    sprintf(filename, "%02d%02d%02dwp.gpx", (year()-2000), month(), day());

    bool fileExists = getTrackerSD()->getSd().exists(filename);
    File32 file = getTrackerSD()->getSd().open(filename, O_WRITE | O_CREAT | O_AT_END);

    if (!file)
    {
        Serial.println("Fehler beim Oeffnen der Datei");
        char messageRow1[20] = "Failed to read file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);
        
        getTrackerSD()->getSd().errorPrint(&Serial);
        Serial.println(getTrackerSD()->getSd().sdErrorCode());
        Serial.println(getTrackerSD()->getSd().sdErrorData());
    }
    else
    {   
        if (!fileExists)
        {
            file.print(
                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                "<gpx version=\"1.1\" creator=\"GpsRouteTracker\">\r\n");
            file.print("</gpx>\r\n");
        }

        unsigned long filesize = file.size();
        // back up the file pointer to just before the closing tags
        filesize -= 8;

        file.seek(filesize);

        file.print(F("<wpt lat=\""));
        file.print(gpsData.latitude, 7);
        file.print(F("\" lon=\""));
        file.print(gpsData.longitude, 7);
        file.println(F("\">"));
        file.print("<name>");
        char time[28];
        sprintf(time, "%02d.%02d.%4d-%02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());
        file.print(time);
        file.println("</name>");
        file.print(F("<time>"));

        if (isSummertime())
        {
            sprintf(time, "%4d-%02d-%02dT%02d:%02d:%02d+02:00", year(), month(), day(), hour(), minute(), second());
        }
        else
        {
            sprintf(time, "%4d-%02d-%02dT%02d:%02d:%02d+01:00", year(), month(), day(), hour(), minute(), second());
        }
        //Serial.println(time);

        file.print(time);
        file.println(F("</time>"));
        file.print(F("<ele>"));
        file.print(gpsData.altitude, 1);
        file.print(F("</ele>\r\n<hdop>"));
        file.print(gpsData.hdpo, 2);
        file.print(F("</hdop>\r\n<course>"));
        file.print(gpsData.course, 1);
        file.print(F("</course>\r\n<speed>"));
        file.print(gpsData.speed, 1);
        file.print(F("</speed>\r\n<sat>"));
        file.print(gpsData.satellites);
        file.println(F("</sat>\r\n</wpt>"));
        file.print("</gpx>\r\n");
        file.close();

        file.flush();
        file.close();
    }
}

void saveDistancesLoop()
{
    unsigned long currentMillis = millis();

    if (gpsData.speed > 0 && (currentMillis - lastDistancesIntervalSave > distancesSaveInterval))
    {
        //Serial.println("saveDistancesLoop interval...");
        saveDistances();
        lastDistancesIntervalSave = currentMillis;
    }
    if (gpsData.stopped > 0 && (currentMillis - lastDistancesStoppedSave > 2000))
    {
        //Serial.println("saveDistancesLoop stopped...");
        saveDistances();
        lastDistancesStoppedSave = currentMillis;
    }
}

void saveLapTimerData()
{
    getTrackerSD()->begin();

    // Delete existing file, otherwise the data is appended to the file
    getTrackerSD()->getSd().remove(lapTimerFilename);

    // Open file for writing
    File32 file = getTrackerSD()->getSd().open(lapTimerFilename, FILE_WRITE);
    if (!file)
    {
        Serial.println(F("Failed to create file"));
        
        char messageRow1[22] = "Failed to create file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);
        
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Set the values in the document
    doc["start_finish_latitude"] = lap.getStartFinishLatitude();
    doc["start_finish_longitude"] = lap.getStartFinishLongitude();
    doc["fastest_lap"] = lap.getFastestLap();
    doc["last_lap"] = lap.getLastLap();

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0)
    {
        Serial.println(F("Failed to write to file"));
        
        char messageRow1[24] = "Failed to write to file";
        char messageRow2[20] = "";
        showMessageScreen(messageRow1, messageRow2);

    }
    else
    {
        Serial.println(F("LapTimerData erfolgreich geschrieben!"));
    }

    // Close the file
    file.flush();
    file.close();
}

void loadLapTimerData()
{
    Serial.println("loadLapTimerData");
    File32 file = getTrackerSD()->getSd().open(lapTimerFilename);

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<512> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        Serial.println(F("Failed to read file LapTimerData"));
    } else {
        //Serial.println("Json in lapTimer überführen...");
        lap.setStartFinishLatitude(doc["start_finish_latitude"]);
        lap.setStartFinishLongitude(doc["start_finish_longitude"]);
        lap.setFastestLap(doc["fastest_lap"]);
        lap.setLastLap(doc["last_lap"]);
    }
    file.close();
}