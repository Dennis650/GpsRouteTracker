#ifndef abstract_screen_compass_h
#define abstract_screen_compass_h

#include "Arduino.h"
#include "AbstractScreen.h"

class AbstractScreenCompass : public AbstractScreen {
public:
   void drawCompass(uint8_t x, uint8_t y, uint8_t r, boolean displaySpeed, boolean displayCourse);
   void drawCompass(uint8_t x, uint8_t y, uint8_t r);
   
   static const int _compass_radius_kreis = 35;
   static const int _compass_size_triangle = 7;
};

#endif