#ifndef screen_fuel_h
#define screen_fuel_h

#include "Arduino.h"
#include "AbstractScreenCompass.h"

static const String SCREEN_FUEL = "ScreenFuel";

class ScreenFuel : public AbstractScreenCompass {
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