#ifndef screen_message_h
#define screen_message_h

#include "Arduino.h"
#include "AbstractScreen.h"

static const String SCREEN_MESSAGE = "ScreenMessage";

class ScreenMessage : public AbstractScreen {
public:
   virtual String getScreenName();

   virtual void drawScreen();
   virtual void handleButtonUpShort();
   virtual bool handleButtonUpLong();
   virtual void handleButtonDownShort();
   virtual bool handleButtonDownLong();
   virtual void handleButtonSelectShort();
   virtual bool handleButtonSelectLong();

   void setMessage(char* messageRow1, char* messageRow2);

private: 
   char* _messageRow1;
   char* _messageRow2;
};

#endif