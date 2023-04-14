#ifndef screen_compass_waypoint_h
#define screen_compass_waypoint_h

#include "Arduino.h"
#include "AbstractScreenCompass.h"

static const String SCREEN_COMPASS_WAYPOINT = "ScreenCompassWaypoint";

class ScreenCompassWaypoint : public AbstractScreenCompass {
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

private:
   void drawDirectionArrow(uint16_t course);
   void drawSubScreenDestination();
   void drawSubScreenHome();
};

#endif