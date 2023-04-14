#include "Arduino.h"
#include "AbstractScreenCompass.h"

void drawDirection(const char *direction, uint8_t x, uint8_t y, uint8_t r, uint16_t course) {
    u8g2.setFont(u8g2_font_helvR10_tf);
    int strWitdh = u8g2.getStrWidth(direction);
    int strHeight = 10;

    course = course + 270;

    uint8_t px = x - r * cos(2 * PI * course / 360);
    uint8_t py = y + r * sin(2 * PI * course / 360);

    px = px - (strWitdh/2);
    py = py + (strHeight/2);

    u8g2.drawStr(px, py, direction);
}

void drawHomeCourse(uint8_t x, uint8_t y, uint8_t r, uint16_t course) {
    course = course + 270;

    uint8_t px = x - r * cos(2 * PI * course / 360);
    uint8_t py = y + r * sin(2 * PI * course / 360);

    u8g2.setFont(u8g2_font_unifont_t_symbols);
    //Symbol ist 16 x 16, centering
    u8g2.drawGlyph(px-8, py+8, 0x2617);	
}

void drawDestinationCourse(uint8_t x, uint8_t y, uint8_t r, uint16_t course) {
    course = course + 270;

    uint8_t px = x - r * cos(2 * PI * course / 360);
    uint8_t py = y + r * sin(2 * PI * course / 360);

    u8g2.setFont(u8g2_font_unifont_t_symbols);
    //Symbol ist 16 x 16, centering
    u8g2.drawGlyph(px-8, py+8, 0x23eb);	
}

void drawDirectionPoint(uint8_t x, uint8_t y, uint8_t r, uint16_t course) {
    course = course + 270;

    uint8_t px = x - r * cos(2 * PI * course / 360);
    uint8_t py = y + r * sin(2 * PI * course / 360);

    u8g2.drawCircle(px, py, 2);
}

void AbstractScreenCompass::drawCompass(uint8_t x, uint8_t y, uint8_t r, boolean displaySpeed, boolean displayCourse) {
    u8g2.drawCircle(x, y, r);

    drawDirection("N", x, y, r+10, gpsData.course + 360 - 0);
    drawDirection("O", x, y, r+10, gpsData.course + 360 - 90);
    drawDirection("S", x, y, r+10, gpsData.course + 360 - 180);
    drawDirection("W", x, y, r+10, gpsData.course + 360 - 270);

    drawDirectionPoint(x, y, r, gpsData.course + 360 - 45);
    drawDirectionPoint(x, y, r, gpsData.course + 360 - 135);
    drawDirectionPoint(x, y, r, gpsData.course + 360 - 225);
    drawDirectionPoint(x, y, r, gpsData.course + 360 - 315);

    if (displayCourse) {
        u8g2.drawTriangle(x, y - r, x - _compass_size_triangle, y - r + _compass_size_triangle, x + _compass_size_triangle, y - r + _compass_size_triangle);

        uint16_t homeCourse = ((int)(360 - gpsData.courseToHome + gpsData.course)) % 360;
        drawHomeCourse(x, y, r, homeCourse);

        uint16_t destinationCourse = ((int)(360 - gpsData.courseToDestination + gpsData.course)) % 360;
        drawDestinationCourse(x, y, r, destinationCourse);
    
        char bufCourse[6];
        char *course = dtostrf(gpsData.course, 0, 0, bufCourse);
        u8g2.setFont(u8g2_font_helvR08_tf);
        int widthCourse = u8g2.getStrWidth(course);
        int xPositionCourse = (128 / 2) - (widthCourse / 2);
        u8g2.drawStr(xPositionCourse, 46, course);
    }

    if (displaySpeed) {
        char bufSpeed[6];
        char *speed = dtostrf(gpsData.speed, 0, 0, bufSpeed);
        u8g2.setFont(u8g2_font_helvB24_tf);
        int widthSpeed = u8g2.getStrWidth(speed);
        int xPositionSpeed = (128 / 2) - (widthSpeed / 2);
        u8g2.drawStr(xPositionSpeed, 76, speed);

        u8g2.setFont(u8g2_font_helvB08_tf);
        int widthKMH = u8g2.getStrWidth("km/h");
        int xPositionKMH = (128 / 2) - (widthKMH / 2);
        u8g2.drawStr(xPositionKMH, 88, "km/h");
    }
}

void AbstractScreenCompass::drawCompass(uint8_t x, uint8_t y, uint8_t r) {
    drawCompass(x, y, r, true, true);
}


