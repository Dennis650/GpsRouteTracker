#ifndef web_functions_h
#define web_functions_h

#include <Arduino.h>

void wifiSetup();
void webSetup();
void setupOTA();
void handleIndex();
bool handleDownload(String uri);
void handleWebClient();
void handleArduinoOTA();
void loopWifiConnect();
void sendFiles2Http();

#endif