#ifndef time_functions_h
#define time_functions_h

#include <Arduino.h>
#include <TinyGPS++.h>

boolean isSummertime();
boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours);
time_t gpsTimeToArduinoTime(TinyGPSDate date, TinyGPSTime time);
char*  getFormattedLapTime(uint32_t time, char* buffer);

#endif