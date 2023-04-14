#include "Arduino.h"
#include "AbstractScreenCompass.h"
#include "ScreenCompass.h"
#include "logos.h"
#include "sd_functions.h"

String ScreenCompass::getScreenName() {
    return SCREEN_COMPASS;
}

void ScreenCompass::drawScreen() {
    u8g2.clearBuffer();
        
    drawTopInfos();

    drawCompass(63, 63, _compass_radius_kreis);

    char bufDistanceTrip1[6];
    char *distanceTrip1Str = dtostrf(distances.getTrip1KM(), 0, 1, bufDistanceTrip1);
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(0, 128, "T1");
    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawStr(16, 128, distanceTrip1Str);

    char bufDistanceFuel[6];
    char *distanceFuelStr = dtostrf(distances.getFuelRemainingKM(), 0, 1, bufDistanceFuel);
    u8g2.setFont(u8g2_font_helvR10_tf);
    int widthDistanceFuel = u8g2.getStrWidth(distanceFuelStr);
    int xPositionDistanceFuel = 128 - widthDistanceFuel;
    u8g2.drawStr(xPositionDistanceFuel, 128, distanceFuelStr);

    u8g2.drawXBMP(xPositionDistanceFuel - 15, 128 - 10, Tankstelle_width, Tankstelle_height, Tankstelle_bits);

    u8g2.sendBuffer();
}


void ScreenCompass::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenCompass::handleButtonUpLong()
{
    trackWaypoint(); 
    messageWaypointSaved();

    return true;
}

void ScreenCompass::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenCompass::handleButtonDownLong()
{
    return true;
}

void ScreenCompass::handleButtonSelectShort()
{

}

bool ScreenCompass::handleButtonSelectLong()
{
    return true;
}

