#ifndef screen_menu_h
#define screen_menu_h

#include "Arduino.h"
#include "AbstractScreen.h"

static const String SCREEN_MENU = "ScreenMenu";

class ScreenMenu : public AbstractScreen {
public:
   virtual String getScreenName();

   virtual void drawScreen();
   virtual void handleButtonUpShort();
   virtual bool handleButtonUpLong();
   virtual void handleButtonDownShort();
   virtual bool handleButtonDownLong();
   virtual void handleButtonSelectShort();
   virtual bool handleButtonSelectLong();

private:
   void nextMenuItem();
   void previousMenuItem();
   void drawMenuEntry();
   void drawMenu();

   int _current_menu_number = 1;
   bool _menu_entered = false;
};

#endif