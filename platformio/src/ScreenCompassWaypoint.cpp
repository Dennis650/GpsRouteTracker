#include "Arduino.h"
#include "AbstractScreenCompass.h"
#include "ScreenCompassWaypoint.h"
#include "TimeLib.h"
#include "sd_functions.h"

String ScreenCompassWaypoint::getScreenName() {
    return SCREEN_COMPASS_WAYPOINT;
}

void ScreenCompassWaypoint::drawDirectionArrow(uint16_t course) {
    uint16_t destinationCourse = course + 270;
    uint8_t px_sptze = 63 - (_compass_radius_kreis - 5) * cos(2 * PI * destinationCourse / 360);
    uint8_t py_sptze = 63 + (_compass_radius_kreis - 5) * sin(2 * PI * destinationCourse / 360);

    uint16_t destinationCourse_rechts = destinationCourse + 45;
    uint8_t px_sptze_rechts = 63 - (_compass_radius_kreis-50) * cos(2 * PI * destinationCourse_rechts / 360);
    uint8_t py_sptze_rechts = 63 + (_compass_radius_kreis-50) * sin(2 * PI * destinationCourse_rechts / 360);

    uint16_t destinationCourse_links = destinationCourse - 45;
    uint8_t px_sptze_links = 63 - (_compass_radius_kreis-50) * cos(2 * PI * destinationCourse_links / 360);
    uint8_t py_sptze_links = 63 + (_compass_radius_kreis-50) * sin(2 * PI * destinationCourse_links / 360);

    u8g2.drawTriangle(px_sptze, py_sptze, px_sptze_rechts, py_sptze_rechts, px_sptze_links, py_sptze_links);
}

void ScreenCompassWaypoint::drawSubScreenDestination() {
    //TODO: remove Redundanzen 
    char time[10];
    sprintf(time, "%02d:%02d", hour(), minute());
    u8g2.setFont(u8g2_font_helvR10_tf);
    int widthTime = u8g2.getStrWidth(time);
    int xPositionTime = 128 - widthTime;
    u8g2.drawStr(xPositionTime, 11, time);

    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawUTF8(0, 11, "-> Dest");

    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawStr(0, 25, "S:");
    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.setCursor(15, 25);
    u8g2.print(gpsData.satellites);

    uint16_t destinationCourse = ((int)(360 - gpsData.courseToDestination + gpsData.course)) % 360;
    drawDirectionArrow(destinationCourse);

    char bufCourseWaypoint[6];
    char *courseWaypointStr = dtostrf(gpsData.courseToDestination, 0, 0, bufCourseWaypoint);
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(0, 128, "Course:");
    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawStr(38, 128, courseWaypointStr);


    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(64, 128, "Dist:");    
    char bufDistanceWaypoint[6];
    char *distanceWaypointStr = dtostrf((gpsData.distanceToDestination / 1000.0), 0, 1, bufDistanceWaypoint);
    u8g2.setFont(u8g2_font_helvR10_tf);
    int widthDistanceWaypoint = u8g2.getStrWidth(distanceWaypointStr);
    int xPositionDistanceWaypoint = 128 - widthDistanceWaypoint;
    u8g2.drawStr(xPositionDistanceWaypoint, 128, distanceWaypointStr);
}

void ScreenCompassWaypoint::drawSubScreenHome() {
    //TODO: remove Redundanzen 
    char time[10];
    sprintf(time, "%02d:%02d", hour(), minute());
    u8g2.setFont(u8g2_font_helvR10_tf);
    int widthTime = u8g2.getStrWidth(time);
    int xPositionTime = 128 - widthTime;
    u8g2.drawStr(xPositionTime, 11, time);

    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawUTF8(0, 11, "-> Home");

    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawStr(0, 25, "S:");
    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.setCursor(15, 25);
    u8g2.print(gpsData.satellites);

    uint16_t destinationCourse = ((int)(360 - gpsData.courseToHome + gpsData.course)) % 360;
    drawDirectionArrow(destinationCourse);

    char bufCourseWaypoint[6];
    char *courseWaypointStr = dtostrf(gpsData.courseToHome, 0, 0, bufCourseWaypoint);
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(0, 128, "Course:");
    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.drawStr(38, 128, courseWaypointStr);


    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(64, 128, "Dist:");    
    char bufDistanceWaypoint[6];
    char *distanceWaypointStr = dtostrf((gpsData.distanceToHome / 1000.0), 0, 1, bufDistanceWaypoint);
    u8g2.setFont(u8g2_font_helvR10_tf);
    int widthDistanceWaypoint = u8g2.getStrWidth(distanceWaypointStr);
    int xPositionDistanceWaypoint = 128 - widthDistanceWaypoint;
    u8g2.drawStr(xPositionDistanceWaypoint, 128, distanceWaypointStr);
}

void ScreenCompassWaypoint::drawScreen() {
    u8g2.clearBuffer();

    drawCompass(64, 64, _compass_radius_kreis, false, false);

    switch (_current_sub_screen) {
        case 0:
            drawSubScreenDestination();
            break;
        case 1:
            drawSubScreenHome();
            break;
    }

    u8g2.sendBuffer();
}

void ScreenCompassWaypoint::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenCompassWaypoint::handleButtonUpLong()
{
    trackWaypoint(); 
    messageWaypointSaved();
    return true;
}

void ScreenCompassWaypoint::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenCompassWaypoint::handleButtonDownLong()
{
    return true;
}

void ScreenCompassWaypoint::handleButtonSelectShort()
{
    _current_sub_screen++;

    if (_current_sub_screen > 1) {
        _current_sub_screen = 0;
    }
}

bool ScreenCompassWaypoint::handleButtonSelectLong()
{
    return true;
}

