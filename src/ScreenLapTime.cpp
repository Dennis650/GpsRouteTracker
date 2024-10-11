#include "Arduino.h"
#include "ScreenLapTime.h"
#include "sd_functions.h"
#include "time_functions.h"

String ScreenLapTime::getScreenName() {
    return SCREEN_LAP_TIME;
}

void ScreenLapTime::drawScreen() {
    u8g2.clearBuffer();
        
    drawTopInfos();

    char bufSpeed[6];
    char *speed = dtostrf(gpsData.speed, 0, 0, bufSpeed);

    u8g2.setFont(u8g2_font_fub30_tn);
    int widthSpeed = u8g2.getStrWidth(speed);
    int xPositionSpeed = (128 / 2) - (widthSpeed / 2);
    u8g2.drawStr(xPositionSpeed, 35, speed);

    u8g2.setFont(u8g2_font_helvB08_tf);
    int widthKMH = u8g2.getStrWidth("km/h");
    int xPositionKMH = 128 - widthKMH;
    u8g2.drawStr(xPositionKMH, 35, "km/h"); 

    u8g2.drawLine(0, 40, 128, 40);

    if (lap.getStartFinishLatitude() == 0 && lap.getStartFinishLongitude() == 0) {
        u8g2.setFont(u8g2_font_helvB10_tf);
        u8g2.drawStr(2, 87, "No start/finish set");
    } else {
        u8g2.setFont(u8g2_font_helvB08_tf);
        u8g2.drawStr(0, 62, "Curr");
        
        char currentLapTime[16];
        getFormattedLapTime(lap.getStopWatch()->elapsed(), currentLapTime);
        u8g2.setFont(u8g2_font_fub17_tn);
        u8g2.drawStr(22, 62, currentLapTime);

        u8g2.drawLine(0, 65, 128, 65);

        if (lap.getLastLap() != 0 && lap.getLastLap() == lap.getFastestLap()) {
            u8g2.setFont(u8g2_font_helvB12_tf);
            u8g2.drawStr(8, 85, "Fastest Lap :-)");
        } else {
            u8g2.setFont(u8g2_font_helvB08_tf);
            u8g2.drawStr(0, 87, "Last");

            char lastLapTime[16];
            getFormattedLapTime(lap.getLastLap(), lastLapTime);
            u8g2.setFont(u8g2_font_fub17_tn);
            u8g2.drawStr(22, 87, lastLapTime);
        }

        u8g2.drawLine(0, 90, 128, 90);

        u8g2.setFont(u8g2_font_helvB08_tf);
        u8g2.drawStr(0, 112, "Fast");

        char fastestLapTime[16];
        getFormattedLapTime(lap.getFastestLap(), fastestLapTime);
        u8g2.setFont(u8g2_font_fub17_tn);
        u8g2.drawStr(22, 112, fastestLapTime);

        u8g2.drawLine(0, 115, 128, 115);
    }

    u8g2.sendBuffer();
}


void ScreenLapTime::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenLapTime::handleButtonUpLong()
{
    if (lap.getStopWatch()->isRunning()) {
        lap.getStopWatch()->stop();
    } 
    lap.getStopWatch()->reset();
    lap.setFastestLap(0);
    lap.setLastLap(0);

    lap.setStartFinishLatitude(gpsData.latitude);
    lap.setStartFinishLongitude(gpsData.longitude);

    char messageRow1[16] = "StartFinish set";
    char messageRow2[16] = "";
    showMessageScreen(messageRow1, messageRow2);

    saveLapTimerData();

    return true;
}

void ScreenLapTime::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenLapTime::handleButtonDownLong()
{
    if (lap.getStopWatch()->isRunning()) {
        lap.getStopWatch()->stop();
    } 
    lap.getStopWatch()->reset();
    lap.setFastestLap(0);
    lap.setLastLap(0);

    saveLapTimerData();

    return true;
}

void ScreenLapTime::handleButtonSelectShort()
{
    if (!lap.getStopWatch()->isRunning()) {
        lap.getStopWatch()->start();
    } else {
        lap.getStopWatch()->stop();
    }
}

bool ScreenLapTime::handleButtonSelectLong()
{
    if (lap.getStopWatch()->isRunning()) {
        lap.getStopWatch()->stop();
    } 
    lap.getStopWatch()->reset();

    return true;
}

