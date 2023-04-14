#ifndef screen_functions_h
#define screen_functions_h

#include <Arduino.h>

void screenSetup();
void screenLoop();

void rotateOled();

void nextScreen();
void previousScreen();

void messageWaypointSaved();
void showMessageScreen(char* message);

String getCurrentScreenName();

#endif