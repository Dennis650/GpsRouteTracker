#include "Arduino.h"
#include "ScreenBoot.h"
#include "bike_logo.h"
#include "version.h"
#include "TimeLib.h"

String ScreenBoot::getScreenName() {
    return SCREEN_BOOT;
}

void ScreenBoot::drawBootLogo() {
    u8g2.drawXBMP(0, 0, bike_logo_width, bike_logo_height, bike_logo_bits);

    String versionString = "Version "+version + " " + versionDate;

    char versionCharArray[50];
    versionString.toCharArray(versionCharArray,50);

    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(0, 128, versionCharArray);
}

void ScreenBoot::drawWaitingForGps() {
    u8g2.drawXBMP(0, 0, bike_logo_width, bike_logo_height, bike_logo_bits);

     char date[32];
     sprintf(date, "%02d.%02d ", day(), month());
     u8g2.setFont(u8g2_font_helvR10_tf);
     u8g2.drawStr(0, 11, date);

     char time[10];
     sprintf(time, "%02d:%02d", hour(), minute());
     u8g2.setFont(u8g2_font_helvR10_tf);
     int widthTime = u8g2.getStrWidth(time);
     int xPositionTime = 128 - widthTime;
     u8g2.drawStr(xPositionTime, 11, time);

     u8g2.setFont(u8g2_font_helvB10_tf);
     u8g2.drawStr(3, 105, "Waiting for GPS...");

     u8g2.setFont(u8g2_font_helvB10_tf);
     u8g2.drawStr(20, 128, "Satellites:");
     u8g2.setFont(u8g2_font_helvR10_tf);
     u8g2.setCursor(92, 128);
     u8g2.print(gpsData.satellites);
}

void ScreenBoot::drawScreen() {
    if (_boot_screen_started_at_millis == 0) {
     _boot_screen_started_at_millis = millis();   
    }

    u8g2.clearBuffer();
    
    if ((millis() - _boot_screen_started_at_millis) < 2000) {
        drawBootLogo();
    } else {
        drawWaitingForGps();
    }
    
    u8g2.sendBuffer();
}

void ScreenBoot::handleButtonUpShort() 
{
}

bool ScreenBoot::handleButtonUpLong()
{
    return true;
}

void ScreenBoot::handleButtonDownShort()
{
}

bool ScreenBoot::handleButtonDownLong()
{
    return true;
}

void ScreenBoot::handleButtonSelectShort()
{
    _deviceReady = true;

}

bool ScreenBoot::handleButtonSelectLong()
{
    return true;
}

bool ScreenBoot::isDeviceReady() {
    if (((millis() - _boot_screen_started_at_millis) > 2000) && (gpsData.latitude > 0 || gpsData.latitude < 0)) {
        _deviceReady = true;
    }

    return _deviceReady;
}