#include "Arduino.h"
#include "AbstractScreenCompass.h"
#include "ScreenCompassTemp.h"
#include "sd_functions.h"
#include "temperature_functions.h"

String ScreenCompassTemp::getScreenName() {
    return SCREEN_COMPASS_TEMP;
}

void ScreenCompassTemp::drawScreen() {
    u8g2.clearBuffer();
        
    drawTopInfos();

    drawCompass(63, 63, _compass_radius_kreis);

    u8g2.setDrawColor(0);
    u8g2.drawBox(0,77,128,51);
    u8g2.setDrawColor(1);

    u8g2.drawLine(0, 77, 128, 77);

    char bufTemperatur[6];
    char *temperatur = dtostrf(getTemperatureSensorValue(), 0, 1, bufTemperatur);
    u8g2.setFont(u8g2_font_fub30_tn);

    int widthTemp = u8g2.getStrWidth(temperatur);
    int xPositionTemp = (128 / 2) - (widthTemp / 2);
    u8g2.drawStr(xPositionTemp, 114 , temperatur);

    u8g2.setFont(u8g2_font_helvB08_tf);
    int widthTempLabel= u8g2.getStrWidth("°C");
    int xPositionTempLabel = (128 / 2) - (widthTempLabel / 2);
    u8g2.drawUTF8(xPositionTempLabel, 128, "°C"); 

    u8g2.sendBuffer();
}


void ScreenCompassTemp::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenCompassTemp::handleButtonUpLong()
{
    trackWaypoint(); 
    messageWaypointSaved();
    return true;
}

void ScreenCompassTemp::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenCompassTemp::handleButtonDownLong()
{
    return true;
}

void ScreenCompassTemp::handleButtonSelectShort()
{

}

bool ScreenCompassTemp::handleButtonSelectLong()
{
    return true;
}

