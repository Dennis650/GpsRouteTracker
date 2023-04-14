#ifndef screen_boot_h
#define screen_boot_h

#include "Arduino.h"
#include "AbstractScreen.h"

static const String SCREEN_BOOT = "ScreenBoot";

class ScreenBoot : public AbstractScreen {
public:
   virtual String getScreenName();

   virtual void drawScreen();
   virtual void handleButtonUpShort();
   virtual bool handleButtonUpLong();
   virtual void handleButtonDownShort();
   virtual bool handleButtonDownLong();
   virtual void handleButtonSelectShort();
   virtual bool handleButtonSelectLong();

   bool isDeviceReady();

private:
   void drawBootLogo();
   void drawWaitingForGps();

   unsigned long _boot_screen_started_at_millis = 0;
   bool _deviceReady = false;
}; 

#endif