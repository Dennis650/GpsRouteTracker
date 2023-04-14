#ifndef sd_functions_h
#define sd_functions_h

#include <Arduino.h>

void loadDistances();
void saveDistances();
void loadTrackerConfig();
void saveTrackerConfig();
void trackPostionLoop();
void trackWaypoint();
void saveDistancesLoop();
void deleteTackerConfig();
void loadLapTimerData();
void saveLapTimerData();

#endif