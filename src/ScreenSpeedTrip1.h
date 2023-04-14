#ifndef screen_compass_speed_trip1_h
#define screen_compass_speed_trip1_h

#include "Arduino.h"
#include "AbstractScreen.h"

static const String SCREEN_SPEED_TRIP1 = "ScreenSpeedTrip1";

class ScreenSpeedTrip1 : public AbstractScreen {
public:
   virtual String getScreenName();

   virtual void drawScreen();
   virtual void handleButtonUpShort();
   virtual bool handleButtonUpLong();
   virtual void handleButtonDownShort();
   virtual bool handleButtonDownLong();
   virtual void handleButtonSelectShort();
   virtual bool handleButtonSelectLong();
};

#endif