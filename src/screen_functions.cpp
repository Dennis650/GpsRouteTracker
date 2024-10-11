#include <Arduino.h>
#include <U8g2lib.h>

#include <button.h>

#include <AbstractScreen.h>
#include <AbstractScreenCompass.h>
#include <ScreenBoot.h>
#include <ScreenCompass.h>
#include <ScreenFuel.h>
#include <ScreenCompassTrips.h>
#include <ScreenCompassWaypoint.h>
#include <ScreenCompassTemp.h>
#include <ScreenSpeedTrip1.h>
#include <ScreenLapTime.h>
#include <ScreenMenu.h>
#include <ScreenMessage.h>
#include <ScreenInfos.h>

ScreenBoot              screenBoot;
ScreenCompass           screenCompass;
ScreenInfos             screenInfos;
ScreenFuel              screenFuel;
ScreenCompassTrips      screenCompassTrips;
ScreenCompassWaypoint   screenCompassWaypoint;
ScreenCompassTemp       screenCompassTemp;
ScreenSpeedTrip1        screenSpeedTrip1;
ScreenLapTime           screenLapTime;
ScreenMenu              screenMenu;
ScreenMessage           screenMessage;
AbstractScreen * const  screens[9]   = {&screenCompass, &screenFuel, &screenCompassTrips, &screenCompassWaypoint, &screenCompassTemp, &screenSpeedTrip1, &screenLapTime, &screenMenu, &screenInfos};

static int              currentScreenNumber = 0;
static unsigned long    show_screen_messages_startetd_at = 0;


const int button_select_pin = 34;
const int button_up_pin = 35;
const int button_down_pin = 36;


AbstractScreen * const getCurrentScreen() {
    if (!screenBoot.isDeviceReady()) {
        return &screenBoot;
    } else {
        return screens[currentScreenNumber];
    }
}

void executeShortPressButtonUp() {
    Serial.println("Button Up pressed short");

    getCurrentScreen()->handleButtonUpShort();
}

bool executeLongPressButtonUp() {
    Serial.println("Button Up pressed long");
    
    return getCurrentScreen()->handleButtonUpLong();
}

void executeShortPressButtonDown() {
    Serial.println("Button Down pressed short");

    getCurrentScreen()->handleButtonDownShort();
}

bool executeLongPressButtonDown() {
    Serial.println("Button down pressed long");

    return getCurrentScreen()->handleButtonDownLong();
}

void executeShortPressButtonSelect() {
    Serial.println("Button Select pressed short");

    getCurrentScreen()->handleButtonSelectShort();
} 

bool executeLongPressButtonSelect() {
    Serial.println("Button select pressed long");

    return getCurrentScreen()->handleButtonSelectLong();
}


DebouncedButton button_up(button_up_pin, "UP", executeShortPressButtonUp, executeLongPressButtonUp);
DebouncedButton button_down(button_down_pin, "DOWN", executeShortPressButtonDown, executeLongPressButtonDown);
DebouncedButton button_select(button_select_pin, "SELECT", executeShortPressButtonSelect, executeLongPressButtonSelect);

String getCurrentScreenName() {
    return getCurrentScreen()->getScreenName();
}

void nextScreen(){
    currentScreenNumber++;

    if (currentScreenNumber >= (sizeof(screens) / sizeof(screens[0]))) {
        currentScreenNumber = 0;
    }
}

void previousScreen(){
    currentScreenNumber--;

    if (currentScreenNumber<0) {
        currentScreenNumber = (sizeof(screens) / sizeof(screens[0])) -1;
    }
}

void showMessageScreen(char* messageRow1, char* messageRow2)
{
    if (show_screen_messages_startetd_at==0) 
    {
        show_screen_messages_startetd_at = millis();
    }
    screenMessage.setMessage(messageRow1, messageRow2);
    screenMessage.drawScreen();
}

void messageWaypointSaved()
{
    char messageRow1[15] = "Waypoint saved";
    char messageRow2[15] = "";
    showMessageScreen(messageRow1, messageRow2);
}

void screenLoop()
{
    button_up.loop();
    button_down.loop();
    button_select.loop();

    if (show_screen_messages_startetd_at != 0) {
        if (millis() - show_screen_messages_startetd_at >= 2000) {
            show_screen_messages_startetd_at = 0;
        }
    } else if (!screenBoot.isDeviceReady()) {
        screenBoot.drawScreen();
    } else {
        screens[currentScreenNumber]->drawScreen();
    }    
}

void screenSetup() 
{
    Serial.println("screenSetup");
    u8g2.begin();

    screenBoot.drawScreen();
}

void rotateOled() {
    if (trackerConfig.displayRotate==0) {
        u8g2.setDisplayRotation(U8G2_R0);
    } else if (trackerConfig.displayRotate==1) {
        u8g2.setDisplayRotation(U8G2_R1);
    } else if (trackerConfig.displayRotate==2) {
        u8g2.setDisplayRotation(U8G2_R2);
    } else if (trackerConfig.displayRotate==3) {
        u8g2.setDisplayRotation(U8G2_R3);
    }
}
