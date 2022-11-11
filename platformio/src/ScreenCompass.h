#ifndef screen_compass_h
#define screen_compass_h

#include "Arduino.h"
#include "AbstractScreenCompass.h"

static const String SCREEN_COMPASS = "ScreenCompass";

class ScreenCompass : public AbstractScreenCompass {
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