#ifndef screen_compass_trips_h
#define screen_compass_trips_h

#include "Arduino.h"
#include "AbstractScreenCompass.h"

static const String SCREEN_COMPASS_TRIPS = "ScreenCompassTrips";

class ScreenCompassTrips : public AbstractScreenCompass {
public:
   virtual String getScreenName();

   virtual void drawScreen();
   virtual void handleButtonUpShort();
   virtual bool handleButtonUpLong();
   virtual void handleButtonDownShort();
   virtual bool handleButtonDownLong();
   virtual void handleButtonSelectShort();
   virtual bool handleButtonSelectLong();

   int _current_sub_screen;
};

#endif