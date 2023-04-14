#include "Arduino.h"
#include "AbstractScreen.h"
#include "time_functions.h"
#include "TimeLib.h"

void AbstractScreen::drawTopInfos() {
    char time[10];
    sprintf(time, "%02d:%02d", hour(), minute());
    u8g2.setFont(u8g2_font_helvR10_tf);
    int widthTime = u8g2.getStrWidth(time);
    int xPositionTime = 128 - widthTime;
    u8g2.drawStr(xPositionTime, 11, time);

    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawUTF8(0, 11, "↑");
    char bufAltitude[6];
    char *altitude = dtostrf(gpsData.altitude, 0, 0, bufAltitude);
    u8g2.setFont(u8g2_font_helvR10_tf);
    int widthAltitude = u8g2.getStrWidth(altitude);
    u8g2.drawUTF8(10, 11, altitude);
    u8g2.drawStr(10 + widthAltitude, 11, "m");

    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawStr(0, 25, "S:");
    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.setCursor(15, 25);
    u8g2.print(gpsData.satellites);
}