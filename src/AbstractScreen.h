#ifndef abstract_screen_h
#define abstract_screen_h

#include "Arduino.h"
#include "screen_functions.h"
#include <U8g2lib.h>
#include <global_vars.h>

class AbstractScreen {
public:
   virtual ~AbstractScreen() {}            // Destructor
   virtual String getScreenName() = 0;
   virtual void drawScreen() = 0;  // Needs to be implemented by each subclass
   virtual void handleButtonUpShort() = 0;
   virtual bool handleButtonUpLong() = 0;
   virtual void handleButtonDownShort() = 0;
   virtual bool handleButtonDownLong() = 0;
   virtual void handleButtonSelectShort() = 0;
   virtual bool handleButtonSelectLong() = 0;

   void drawTopInfos();
};

#endif