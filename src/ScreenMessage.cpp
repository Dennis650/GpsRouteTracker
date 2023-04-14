#include "Arduino.h"
#include "ScreenMessage.h"
#include "sd_functions.h"

String ScreenMessage::getScreenName() {
    return SCREEN_MESSAGE;
}

void ScreenMessage::drawScreen() {
    u8g2.clearBuffer();
    if (sizeof(_message) > 18) {
        u8g2.setFont(u8g2_font_helvB08_tf);
    } else {
        u8g2.setFont(u8g2_font_helvB12_tf);
    }
    u8g2.setFont(u8g2_font_helvB12_tf);
    int widthMessage = u8g2.getStrWidth(_message);
    int xPositionMessage = (128 / 2) - (widthMessage / 2);
    u8g2.drawStr(xPositionMessage, 76, _message);
    u8g2.sendBuffer();
}


void ScreenMessage::handleButtonUpShort() 
{
}

bool ScreenMessage::handleButtonUpLong()
{
   return true;
}

void ScreenMessage::handleButtonDownShort()
{
}

bool ScreenMessage::handleButtonDownLong()
{
    return true;
}

void ScreenMessage::handleButtonSelectShort()
{
}

bool ScreenMessage::handleButtonSelectLong()
{
    return true;
}

void ScreenMessage::setMessage(char* message) {
    _message = message;
}

