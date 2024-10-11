#include "Arduino.h"
#include "ScreenMessage.h"
#include "sd_functions.h"

String ScreenMessage::getScreenName() {
    return SCREEN_MESSAGE;
}

void ScreenMessage::drawScreen() {
    u8g2.clearBuffer();
    if (sizeof(_messageRow1) > 18) {
        u8g2.setFont(u8g2_font_helvB08_tf);
    } else {
        u8g2.setFont(u8g2_font_helvB12_tf);
    }
    int widthMessageRow1 = u8g2.getStrWidth(_messageRow1);
    int xPositionMessageRow1 = (128 / 2) - (widthMessageRow1 / 2);
    u8g2.drawStr(xPositionMessageRow1, 64, _messageRow1);
    u8g2.sendBuffer();

    if (sizeof(_messageRow2) > 18) {
        u8g2.setFont(u8g2_font_helvB08_tf);
    } else {
        u8g2.setFont(u8g2_font_helvB12_tf);
    }
    int widthMessageRow2 = u8g2.getStrWidth(_messageRow2);
    int xPositionMessageRow2 = (128 / 2) - (widthMessageRow2 / 2);
    u8g2.drawStr(xPositionMessageRow2, 84, _messageRow2);
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

void ScreenMessage::setMessage(char* messageRow1, char* messageRow2) {
    _messageRow1 = messageRow1;
    _messageRow2 = messageRow2;
}

