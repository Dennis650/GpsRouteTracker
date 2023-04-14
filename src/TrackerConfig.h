#include <Arduino.h>

#ifndef tracker_config_h
#define tracker_config_h

struct TrackerConfig
{
    double tankinhalt;             
    double tankreserve;
    double durchschnittsverbrauch;

    //WLAN + Bluetooth
    char wlanMode[10]="AP";
    char wlanApSsid[50];
    char wlanApPassword[50]; 
    char wlanClientSsid[50];
    char wlanClientPassword[50]; 

    char device[50];

    int displayRotate;

    double home_latitude;
    double home_longitude;

    double destination_latitude;
    double destination_longitude;
};

#endif