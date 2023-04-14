#ifndef screen_laptime_h
#define screen_laptime_h

#include "Arduino.h"
#include "AbstractScreen.h"

static const String SCREEN_LAP_TIME = "ScreenLapTime";

class ScreenLapTime : public AbstractScreen {
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