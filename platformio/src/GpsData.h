#include <Arduino.h>

#ifndef gps_data_h
#define gps_data_h

struct GpsData
{
    bool location_valid = false;    
    int satellites;
    float latitude;
    float longitude;
    float latitude_previous;
    float longitude_previous;
    double speed;
    double speed_previous = 0;
    double altitude;
    double course;
    double hdpo;
    unsigned long distanceMetersSinceLastFix;
    bool stopped = false;
    bool wiringError = false;
    unsigned long distanceToHome;
    double courseToHome;
    unsigned long distanceToDestination;
    double courseToDestination;
};

#endif