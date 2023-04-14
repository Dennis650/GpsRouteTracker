#ifndef screen_infos_h
#define screen_infos_h

#include "Arduino.h"
#include "AbstractScreen.h"

static const String SCREEN_INFOS = "ScreenInfos";

class ScreenInfos : public AbstractScreen {
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