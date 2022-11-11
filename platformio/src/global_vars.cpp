#include <Arduino.h>
#include <Distances.h>
#include <TrackerGps.h>
#include <TrackerConfig.h>
#include <GpsData.h>
#include <Lap.h>
#include <global_vars.h>

TrackerConfig trackerConfig;
GpsData gpsData;
Distances distances;

TrackerGps trackerGps(16,17,9600);

Lap lap;

//OLED Display
U8G2_SSD1327_WS_128X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/4, /* dc=*/32, /* reset=*/33);

