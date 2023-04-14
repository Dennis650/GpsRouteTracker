#ifndef TrackerGps_h
#define TrackerGps_h

#include <Arduino.h>
#include <TinyGPS++.h>
#include <TrackerConfig.h>
#include <GpsData.h>
#include <Distances.h>

class TrackerGps
{
  public:
    TrackerGps(int rxPin, int txPin, uint32_t gpsSerialBaud);
    void begin();
    void readSerialGPS();
    void gpsFetchDataAndCalcDistances();
    static const char* getCardinal(double deg);
    static const char* getCardinal();
    const char* getCurrentCardinal();
    static void gpsPrintInfos();
  private:
    int _rxPin;
    int _txPin;
    uint32_t _gpsSerialBaud;
    TinyGPSPlus _tinyGpsPlus;
};

#endif