#ifndef global_vars_h
#define global_vars_h

#include <Arduino.h>
#include <Distances.h>
#include <TrackerGps.h>
#include <TrackerConfig.h>
#include <GpsData.h>
#include <Lap.h>
#include <U8g2lib.h>

extern TrackerConfig trackerConfig;
extern GpsData gpsData;
extern Distances distances;
extern TrackerGps trackerGps;
extern Lap lap;
extern U8G2_SSD1327_WS_128X128_F_4W_HW_SPI u8g2;

#endif