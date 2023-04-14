#include "Arduino.h"
#include "AbstractScreenCompass.h"
#include "ScreenFuel.h"
#include "sd_functions.h"

String ScreenFuel::getScreenName() {
    return SCREEN_FUEL;
}

void drawSubScreenRemaining() {
    u8g2.setDrawColor(0);
    u8g2.drawBox(0,77,128,51);
    u8g2.setDrawColor(1);

    u8g2.drawLine(0, 77, 128, 77);

    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, 100, "Stop in");
    char bufDistanceFuel[6];
    char *distanceFuelStr = dtostrf(distances.getFuelRemainingKM(), 0, 1, bufDistanceFuel);
    u8g2.setFont(u8g2_font_helvR18_tf);
    int widthDistanceFuel = u8g2.getStrWidth(distanceFuelStr);
    int xPositionDistanceFuel = 128 - widthDistanceFuel;
    u8g2.drawStr(xPositionDistanceFuel, 100, distanceFuelStr);


    double fuelWithoutReserve = trackerConfig.tankinhalt - trackerConfig.tankreserve;

    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, 127, "Rest fuel");
    char bufRemainingFuel[6];
    char *remainingFuelStr = dtostrf(fuelWithoutReserve - (trackerConfig.durchschnittsverbrauch * distances.getFuelKM() / 100), 0, 1, bufRemainingFuel);
    u8g2.setFont(u8g2_font_helvR18_tf);
    int widthRemainingFuel = u8g2.getStrWidth(remainingFuelStr);
    int xPositionRemainingFuel = 128 - widthRemainingFuel;
    u8g2.drawStr(xPositionRemainingFuel, 127, remainingFuelStr);
}

void drawSubScreenWithReserve() {
    u8g2.setDrawColor(0);
    u8g2.drawBox(0,77,128,51);
    u8g2.setDrawColor(1);

    u8g2.drawLine(0, 77, 128, 77);

    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, 100, "Empty in");
    char bufDistanceFuel[6];
    char *distanceFuelStr = dtostrf(distances.getFuelRemainingKMWithReserve(), 0, 1, bufDistanceFuel);
    u8g2.setFont(u8g2_font_helvR18_tf);
    int widthDistanceFuel = u8g2.getStrWidth(distanceFuelStr);
    int xPositionDistanceFuel = 128 - widthDistanceFuel;
    u8g2.drawStr(xPositionDistanceFuel, 100, distanceFuelStr);


    double fuelWithReserve = trackerConfig.tankinhalt;

    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, 127, "Reserve");
    char bufRemainingFuel[6];
    char *remainingFuelStr = dtostrf(fuelWithReserve - (trackerConfig.durchschnittsverbrauch * distances.getFuelKM() / 100), 0, 1, bufRemainingFuel);
    u8g2.setFont(u8g2_font_helvR18_tf);
    int widthRemainingFuel = u8g2.getStrWidth(remainingFuelStr);
    int xPositionRemainingFuel = 128 - widthRemainingFuel;
    u8g2.drawStr(xPositionRemainingFuel, 127, remainingFuelStr);
}

void drawSubScreenSince() {
    u8g2.setDrawColor(0);
    u8g2.drawBox(0,77,128,51);
    u8g2.setDrawColor(1);

    u8g2.drawLine(0, 77, 128, 77);

    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, 100, "last stop");
    char bufDistanceFuel[6];
    char *distanceFuelStr = dtostrf(distances.getFuelKM(), 0, 1, bufDistanceFuel);
    u8g2.setFont(u8g2_font_helvR18_tf);
    int widthDistanceFuel = u8g2.getStrWidth(distanceFuelStr);
    int xPositionDistanceFuel = 128 - widthDistanceFuel;
    u8g2.drawStr(xPositionDistanceFuel, 100, distanceFuelStr);

    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, 127, "consumed");
    char bufRemainingFuel[6];
    char *remainingFuelStr = dtostrf(trackerConfig.durchschnittsverbrauch * distances.getFuelKM() / 100, 0, 1, bufRemainingFuel);
    u8g2.setFont(u8g2_font_helvR18_tf);
    int widthRemainingFuel = u8g2.getStrWidth(remainingFuelStr);
    int xPositionRemainingFuel = 128 - widthRemainingFuel;
    u8g2.drawStr(xPositionRemainingFuel, 127, remainingFuelStr);
}

void ScreenFuel::drawScreen() {
    u8g2.clearBuffer();

    drawTopInfos();

    drawCompass(64, 64, _compass_radius_kreis);

    switch (_current_sub_screen) {
        case 0:
            drawSubScreenRemaining();
            break;
        case 1:
            drawSubScreenWithReserve();
            break;
        case 2:
            drawSubScreenSince();
            break;
    }

    u8g2.sendBuffer();
}

void ScreenFuel::handleButtonUpShort() 
{
    nextScreen();
}

bool ScreenFuel::handleButtonUpLong()
{
    trackWaypoint(); 
    messageWaypointSaved();
    return true;
}

void ScreenFuel::handleButtonDownShort()
{
    previousScreen();
}

bool ScreenFuel::handleButtonDownLong()
{
    return true;
}

void ScreenFuel::handleButtonSelectShort()
{
    _current_sub_screen++;

    if (_current_sub_screen > 2) {
        _current_sub_screen = 0;
    }
}

bool ScreenFuel::handleButtonSelectLong()
{
    distances.setFuel(0.0);
    saveDistances();
    return true;
}

