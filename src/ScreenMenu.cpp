#include "Arduino.h"
#include "ScreenMenu.h"
#include "sd_functions.h"
#include "web_functions.h"
#include <WiFi.h>

String ScreenMenu::getScreenName() {
    return SCREEN_MENU;
}

void ScreenMenu::nextMenuItem(){
    _current_menu_number++;
    if (_current_menu_number>5) {
        _current_menu_number=5;
    }
}

void ScreenMenu::previousMenuItem(){
    _current_menu_number--;
    if (_current_menu_number<1) {
        _current_menu_number=1;
    }
}

void ScreenMenu::drawMenu() 
{
    int y_position = 0;
    int line_height = 14;

    y_position = 12;
    u8g2.setFont(u8g2_font_helvB12_tf);
    u8g2.drawStr(0, y_position, "Menu");

    if(strcmp(trackerConfig.wlanMode, "CLIENT") == 0) {
        if (WiFi.status() == WL_CONNECTED) {
            u8g2.setFont(u8g2_font_helvR08_tf);
            u8g2.drawStr(0, 128, "WLAN connected");
        } else {
                        u8g2.setFont(u8g2_font_helvR08_tf);
            u8g2.drawStr(0, 128, "WLAN not connected");
        }
    }

    u8g2.drawLine(0, 15, 127, 15);
    y_position += 5;

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(8, y_position, "WlanMode: ");
    u8g2.setFont(u8g2_font_helvR10_tf);
    u8g2.setCursor(90, y_position);
    u8g2.print(trackerConfig.wlanMode);

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(8, y_position, "Upload files");

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(8, y_position, "!restart!");

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(8, y_position, "!reset config!");

    y_position += line_height;
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(8, y_position, "return");
    
    //current Item malen
    u8g2.setFont(u8g2_font_helvB10_tf);
    u8g2.drawStr(0, (_current_menu_number*line_height)+17, ">");
}

void ScreenMenu::drawMenuEntry()
{
    char menu[5] = "Menu";
    u8g2.setFont(u8g2_font_helvB24_tf);
    int widthMenu = u8g2.getStrWidth(menu);
    int xPositionMenu = (128 / 2) - (widthMenu / 2);
    u8g2.drawStr(xPositionMenu, 50, menu);

    char hint[16] = "select to enter";
    u8g2.setFont(u8g2_font_helvB12_tf);
    int widthhint = u8g2.getStrWidth(hint);
    int xPositionHint = (128 / 2) - (widthhint / 2);
    u8g2.drawStr(xPositionHint, 80, hint);
}

void ScreenMenu::drawScreen() {
    u8g2.clearBuffer();
        
    if (_menu_entered) {
        drawMenu();
    } else {
        drawMenuEntry();
    }

    u8g2.sendBuffer();
}


void ScreenMenu::handleButtonUpShort() 
{
    if (_menu_entered) {
        previousMenuItem();
    } else {
        nextScreen();
    }
}

bool ScreenMenu::handleButtonUpLong()
{
   return true;
}

void ScreenMenu::handleButtonDownShort()
{
    if (_menu_entered) {
        nextMenuItem();
    } else {
        previousScreen();
    }
}

bool ScreenMenu::handleButtonDownLong()
{
    return true;
}

void ScreenMenu::handleButtonSelectShort()
{
    if (_menu_entered) {
        switch (_current_menu_number) {   
            case 1:
                if(strcmp(trackerConfig.wlanMode, "AP") == 0) {
                    strlcpy(trackerConfig.wlanMode, "CLIENT", sizeof(trackerConfig.wlanMode)); 
                } else if(strcmp(trackerConfig.wlanMode, "CLIENT") == 0) {
                    strlcpy(trackerConfig.wlanMode, "OFF", sizeof(trackerConfig.wlanMode)); 
                } else if(strcmp(trackerConfig.wlanMode, "OFF") == 0) {
                    strlcpy(trackerConfig.wlanMode, "AP", sizeof(trackerConfig.wlanMode)); 
                }
                saveTrackerConfig();
                break;
            case 2: 
                sendFiles2Http();
                _menu_entered = false;
                _current_menu_number = 1;
                break;
            case 3: 
                ESP.restart();
                break;
            case 4: 
                deleteTackerConfig();
                ESP.restart();
                break;
            case 5: 
                _menu_entered = false;
                _current_menu_number = 1;
                break;
            default:
                break;
        }
    } else {
        _menu_entered = true;
    }
}

bool ScreenMenu::handleButtonSelectLong()
{
    return true;
}

