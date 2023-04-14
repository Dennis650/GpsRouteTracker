#include <Arduino.h>
#include <TinyGPS++.h>
#include <TrackerGps.h>
#include <time_functions.h>
#include <TimeLib.h>
#include <TrackerConfig.h>
#include <GpsData.h>
#include <global_vars.h>
#include <Distances.h>
#include <screen_functions.h>
#include <ScreenLapTime.h>
#include <sd_functions.h>

HardwareSerial SerialGPS(1);

static unsigned long lastSerialOutputGpsInfos = 0;

TrackerGps::TrackerGps(int rxPin, int txPin, uint32_t gpsSerialBaud) {
    Serial.println("Constructor TrackerGps");
    _rxPin = rxPin;
    _txPin = txPin;
    _gpsSerialBaud = gpsSerialBaud;
}

void TrackerGps::begin() 
{
    Serial.println("TrackerGps::begin");
    SerialGPS.begin(_gpsSerialBaud, SERIAL_8N1, _rxPin, _txPin);
}

void TrackerGps::readSerialGPS()
{
    while (SerialGPS.available())
    {
        char c = SerialGPS.read();
        //Serial.print(c);
        _tinyGpsPlus.encode(c);
    }
    //Serial.println();
}

void TrackerGps::gpsFetchDataAndCalcDistances()
{
    gpsData.location_valid = _tinyGpsPlus.location.isValid();

    if (_tinyGpsPlus.satellites.isValid() && _tinyGpsPlus.satellites.isUpdated())
    {
        gpsData.satellites = _tinyGpsPlus.satellites.value();
    }

    if (_tinyGpsPlus.location.isValid() && _tinyGpsPlus.location.isUpdated())
    {
        gpsData.latitude_previous = gpsData.latitude;
        gpsData.longitude_previous = gpsData.longitude;
        gpsData.latitude = _tinyGpsPlus.location.lat();
        gpsData.longitude = _tinyGpsPlus.location.lng();

        if (gpsData.latitude_previous != 0 && gpsData.longitude_previous != 0)
        {
            gpsData.distanceMetersSinceLastFix =
             (unsigned long)TinyGPSPlus::distanceBetween(
                    gpsData.latitude_previous,
                    gpsData.longitude_previous,
                    gpsData.latitude,
                    gpsData.longitude);

            distances.addDistance(gpsData.distanceMetersSinceLastFix);

            //Calc Distance to Lap start/finish
            double distanceToLapStartFinish = (unsigned long)TinyGPSPlus::distanceBetween(
                    lap.getStartFinishLatitude(),
                    lap.getStartFinishLongitude(),
                    gpsData.latitude,
                    gpsData.longitude);

            bool newLap = lap.executeCurrentDistanceToStartFinish(distanceToLapStartFinish);
            if (newLap) {
                saveLapTimerData();
            }

            //Calc Distance to next Home
            gpsData.distanceToHome = (
            (unsigned long)TinyGPSPlus::distanceBetween(
                    trackerConfig.home_latitude,
                    trackerConfig.home_longitude,
                    gpsData.latitude,
                    gpsData.longitude));

            //Calc Course to next Home
            gpsData.courseToHome = TinyGPSPlus::courseTo(
                    gpsData.latitude,
                    gpsData.longitude,
                    trackerConfig.home_latitude,
                    trackerConfig.home_longitude);

            //Calc Distance to next Destination
            gpsData.distanceToDestination = (
            (unsigned long)TinyGPSPlus::distanceBetween(
                    trackerConfig.destination_latitude,
                    trackerConfig.destination_longitude,
                    gpsData.latitude,
                    gpsData.longitude));

            //Calc Course to next Destination
            gpsData.courseToDestination = TinyGPSPlus::courseTo(
                    gpsData.latitude,
                    gpsData.longitude,
                    trackerConfig.destination_latitude,
                    trackerConfig.destination_longitude);
        }
    }

    if (_tinyGpsPlus.time.isValid() && _tinyGpsPlus.date.isValid())
    {
        gpsTimeToArduinoTime(_tinyGpsPlus.date, _tinyGpsPlus.time);
    }

    if (_tinyGpsPlus.speed.isValid() && _tinyGpsPlus.speed.isUpdated())
    {
        gpsData.speed = _tinyGpsPlus.speed.kmph();
        if (gpsData.speed < 4)
        {
            gpsData.speed = 0;
        }

        if (gpsData.speed_previous > 0 && gpsData.speed == 0)
        {
            gpsData.stopped = true;
        }
        else
        {
            gpsData.stopped = false;
        }

        if (gpsData.speed_previous == 0 && gpsData.speed > 0) {
            //started to move
            if (getCurrentScreenName() == SCREEN_LAP_TIME) {
                if (lap.isPositionInStartFinishZone() && !lap.getStopWatch()->isRunning()) {
                    lap.getStopWatch()->start();
                }
            }
        }

        gpsData.speed_previous = gpsData.speed;
    }

    if (_tinyGpsPlus.altitude.isValid() && _tinyGpsPlus.altitude.isUpdated())
    {
        gpsData.altitude = _tinyGpsPlus.altitude.meters();
    }

    if (_tinyGpsPlus.course.isValid() && _tinyGpsPlus.course.isUpdated())
    {
        gpsData.course = _tinyGpsPlus.course.deg();
    }

    if (_tinyGpsPlus.hdop.isValid() && _tinyGpsPlus.hdop.isUpdated())
    {
        gpsData.hdpo = _tinyGpsPlus.hdop.hdop();
    }

    if (millis() > 10000 && _tinyGpsPlus.charsProcessed() < 10)
    {
        Serial.println(F("No GPS data received: check wiring"));
        char message[17] = "GPS wiring error";
        showMessageScreen(message);

        gpsData.wiringError = true;
    }
    else
    {
        gpsData.wiringError = false;
    }
}

const char* TrackerGps::getCardinal(double deg)
{
    return TinyGPSPlus::cardinal(deg);
}

const char* TrackerGps::getCardinal()
{
    return getCardinal(gpsData.course);
}

const char* TrackerGps::getCurrentCardinal()
{
    return getCardinal(_tinyGpsPlus.course.deg());
}

void TrackerGps::gpsPrintInfos()
{
    unsigned long currentMillis = millis();

    if (currentMillis - lastSerialOutputGpsInfos >= 1000)
    {
        lastSerialOutputGpsInfos = currentMillis;
        Serial.print(" HDPO: ");
        Serial.print(gpsData.hdpo);
        Serial.print(" Satellites: ");
        Serial.print(gpsData.satellites);
        Serial.print(" Lat: ");
        Serial.print(gpsData.latitude, 6);
        Serial.print(" Long: ");
        Serial.print(gpsData.longitude, 6);
        Serial.print(" Speed: ");
        Serial.print(gpsData.speed, 1);
        Serial.print(" Course: ");
        Serial.print(gpsData.course, 1);
        Serial.print(" Time: ");
        char time[32];
        sprintf(time, "%02d:%02d:%02d ", hour(), minute(), second());
        Serial.print(time);
        Serial.print(" Date: ");
        char datum[32];
        sprintf(datum, "%02d.%02d.%02d ", day(), month(), year());
        Serial.print(datum);

        Serial.println();

        Serial.print("ODO: ");
        Serial.print(distances.getOdoKM());
        Serial.print(" Trip1 : ");
        Serial.print(distances.getTrip1KM());
        Serial.print(" Trip2 : ");
        Serial.print(distances.getTrip2KM());
        Serial.print(" Fuel: ");
        Serial.print(distances.getFuelRemainingKM());
        Serial.println();
    }
}
