#include "Arduino.h"
#include "ScreenInfos.h"
#include "TrackerGps.h"
#include "temperature_functions.h"
#include "time_functions.h"
#include "TimeLib.h"
#include "sd_functions.h"

String ScreenInfos::getScreenName() {
    return SCREEN_INFOS;
}

void ScreenInfos::drawScreen() {
    u8g2.clearBuffer();

    int y_position = 0;
    int line_height = 10;

    y_position = 12;
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, y_position, "Infos");
    u8g2.drawLine(0, 13, 127, 13);
    y_position += 3;

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Satellites:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(gpsData.satellites);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "HDPO:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(gpsData.hdpo, 2);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Date:");
    char date[32];
    sprintf(date, "%02d.%02d.%02d ", day(), month(), year());
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(63, y_position, date);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Time:");
    char time[32];
    sprintf(time, "%02d:%02d:%02d ", hour(), minute(), second());
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(63, y_position, time);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Latitude:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(gpsData.latitude, 6);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Longitude:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(gpsData.longitude, 6);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Altitude:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(gpsData.altitude, 1);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Speed:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(gpsData.speed, 1);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Course:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(gpsData.course, 1);
    u8g2.drawStr(95, y_position, TrackerGps::getCardinal());

    float temperatur = getTemperatureSensorValue();
    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB08_tf);
    u8g2.drawStr(0, y_position, "Temperatur:");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.setCursor(63, y_position);
    u8g2.print(temperatur, 1);

    u8g2.sendBuffer();
}

void ScreenInfos::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenInfos::handleButtonUpLong()
{
    trackWaypoint(); 
    messageWaypointSaved();
    return true;
}

void ScreenInfos::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenInfos::handleButtonDownLong()
{
    return true;
}

void ScreenInfos::handleButtonSelectShort()
{

}

bool ScreenInfos::handleButtonSelectLong()
{
    return true;
}
