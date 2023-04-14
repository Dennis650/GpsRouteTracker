#include "Arduino.h"
#include "ScreenSpeedTrip1.h"
#include "sd_functions.h"
#include "TimeLib.h"

String ScreenSpeedTrip1::getScreenName() {
    return SCREEN_SPEED_TRIP1;
}

void ScreenSpeedTrip1::drawScreen() {
    u8g2.clearBuffer();
        
    char bufSpeed[6];
    char *speed = dtostrf(gpsData.speed, 0, 0, bufSpeed);

    u8g2.setFont(u8g2_font_fub49_tn);
    int widthSpeed = u8g2.getStrWidth(speed);
    int xPositionSpeed = (128 / 2) - (widthSpeed / 2);
    u8g2.drawStr(xPositionSpeed, 50, speed);

    u8g2.setFont(u8g2_font_helvB08_tf);
    int widthKMH = u8g2.getStrWidth("km/h");
    int xPositionKMH = (128 / 2) - (widthKMH / 2);
    u8g2.drawStr(xPositionKMH, 63, "km/h"); 

    u8g2.drawLine(0, 68, 128, 68);

    u8g2.setFont(u8g2_font_helvB14_tf);
    u8g2.drawStr(0, 99, "T1");
    char bufDistanceTrip1[10];
    char *distanceTrip1Str = dtostrf(distances.getTrip1KM(), 0, 1, bufDistanceTrip1);
    u8g2.setFont(u8g2_font_helvR24_tf);
    int widthDistanceTrip1 = u8g2.getStrWidth(distanceTrip1Str);
    int xPositionDistanceTrip1 = 128 - widthDistanceTrip1;
    u8g2.drawStr(xPositionDistanceTrip1, 99, distanceTrip1Str);

    u8g2.drawLine(0, 104, 128, 104);

    char time[10];
    sprintf(time, "%02d:%02d", hour(), minute());
    u8g2.setFont(u8g2_font_helvR18_tf);
    int widthTime = u8g2.getStrWidth(time);
    int xPositionTime = (128 / 2) - (widthTime / 2);
    u8g2.drawStr(xPositionTime, 127, time);
    u8g2.sendBuffer();
}


void ScreenSpeedTrip1::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenSpeedTrip1::handleButtonUpLong()
{
    trackWaypoint(); 
    messageWaypointSaved();
    return true;
}

void ScreenSpeedTrip1::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenSpeedTrip1::handleButtonDownLong()
{
    return true;
}

void ScreenSpeedTrip1::handleButtonSelectShort()
{

}

bool ScreenSpeedTrip1::handleButtonSelectLong()
{
    distances.setTrip1(0.0);
    saveDistances();
    return true;
}

