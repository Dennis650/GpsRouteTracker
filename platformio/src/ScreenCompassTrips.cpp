#include "Arduino.h"
#include "AbstractScreenCompass.h"
#include "ScreenCompassTrips.h"
#include "sd_functions.h"

String ScreenCompassTrips::getScreenName() {
    return SCREEN_COMPASS_TRIPS;
}

//TODO: Avaerage Speed fix / implement
void ScreenCompassTrips::drawScreen() {
    u8g2.clearBuffer();

    drawTopInfos();

    drawCompass(64, 64, _compass_radius_kreis);

    u8g2.setDrawColor(0);
    u8g2.drawBox(0,77,128,51);
    u8g2.setDrawColor(1);

    u8g2.drawLine(0, 77, 128, 77);

    u8g2.setFont(u8g2_font_helvB14_tf);
    if (_current_sub_screen==0 || _current_sub_screen==2) {
        u8g2.drawStr(0, 100, ">T1");
    } else {
        u8g2.drawStr(0, 100, " T1");
    }

    if (_current_sub_screen<2) {
        char bufDistanceTrip1[10];
        char *distanceTrip1Str = dtostrf(distances.getTrip1KM(), 0, 1, bufDistanceTrip1);
        u8g2.setFont(u8g2_font_helvR18_tf);
        int widthDistanceTrip1 = u8g2.getStrWidth(distanceTrip1Str);
        int xPositionDistanceTrip1 = 112 - widthDistanceTrip1;
        u8g2.drawStr(xPositionDistanceTrip1, 100, distanceTrip1Str);
        
        u8g2.setFont(u8g2_font_helvR08_tf);
        int widthKm = u8g2.getStrWidth("km");
        int xPositionKm = 128 - widthKm;
        u8g2.drawStr(xPositionKm, 100, "km");
    } else {
        char bufOverageSpeedTrip1[10];
        char *overageSpeedTrip1Str = dtostrf(distances.getTrip1OverageSpeed(), 0, 1, bufOverageSpeedTrip1);
        u8g2.setFont(u8g2_font_helvR18_tf);
        int widthOverageSpeedTrip1 = u8g2.getStrWidth(overageSpeedTrip1Str);
        int xPositionOverageSpeedTrip1 = 105 - widthOverageSpeedTrip1;
        u8g2.drawStr(xPositionOverageSpeedTrip1, 100, overageSpeedTrip1Str);

        u8g2.setFont(u8g2_font_helvR08_tf);
        int widthKmh = u8g2.getStrWidth("km/h");
        int xPositionKmh = 128 - widthKmh;
        u8g2.drawStr(xPositionKmh, 100, "km/h");
    }


    u8g2.setFont(u8g2_font_helvB14_tf);
    if (_current_sub_screen==1 || _current_sub_screen==3) {
        u8g2.drawStr(0, 127, ">T2");
    } else {
        u8g2.drawStr(0, 127, " T2");
    }

    if (_current_sub_screen<2) {
        char bufDistanceTrip2[10];
        char *distanceTrip2Str = dtostrf(distances.getTrip2KM(), 0, 1, bufDistanceTrip2);
        u8g2.setFont(u8g2_font_helvR18_tf);
        int widthDistanceTrip2 = u8g2.getStrWidth(distanceTrip2Str);
        int xPositionDistanceTrip2 = 112 - widthDistanceTrip2;
        u8g2.drawStr(xPositionDistanceTrip2, 127, distanceTrip2Str);

        u8g2.setFont(u8g2_font_helvR08_tf);
        int widthKm = u8g2.getStrWidth("km");
        int xPositionKm = 128 - widthKm;
        u8g2.drawStr(xPositionKm, 127, "km");
    } else {
        char bufOverageSpeedTrip2[10];
        char *overageSpeedTrip2Str = dtostrf(distances.getTrip2OverageSpeed(), 0, 1, bufOverageSpeedTrip2);
        u8g2.setFont(u8g2_font_helvR18_tf);
        int widthOverageSpeedTrip2 = u8g2.getStrWidth(overageSpeedTrip2Str);
        int xPositionOverageSpeedTrip2 = 105 - widthOverageSpeedTrip2;
        u8g2.drawStr(xPositionOverageSpeedTrip2, 127, overageSpeedTrip2Str);

        u8g2.setFont(u8g2_font_helvR08_tf);
        int widthKmh = u8g2.getStrWidth("km/h");
        int xPositionKmh = 128 - widthKmh;
        u8g2.drawStr(xPositionKmh, 127, "km/h");
    }

    u8g2.sendBuffer();
}

void ScreenCompassTrips::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenCompassTrips::handleButtonUpLong()
{
    trackWaypoint(); 
    messageWaypointSaved();
    return true;
}

void ScreenCompassTrips::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenCompassTrips::handleButtonDownLong()
{
    return true;
}

void ScreenCompassTrips::handleButtonSelectShort()
{
    _current_sub_screen++;

    if (_current_sub_screen > 3) {
        _current_sub_screen = 0;
    }
}

bool ScreenCompassTrips::handleButtonSelectLong()
{
    if (_current_sub_screen==0) {
        distances.setTrip1(0.0);
        distances.setTrip1StartMillis(millis());
    } else if (_current_sub_screen==1) {
        distances.setTrip2(0.0);
        distances.setTrip2StartMillis(millis());
    }
    saveDistances();
    return true;
}

