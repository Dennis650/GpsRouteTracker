#ifndef button_h
#define button_h

#include <Arduino.h>

const unsigned int debounceDelayLow=30;
const unsigned int debounceDelayHigh=50;
const unsigned int button_hold_threshold=500; 

class DebouncedButton {
public:
	DebouncedButton(int pin, String name, void(*funcShortPress)(void), bool(*funcButtonHold)());
	void loop();
    bool isButtonHeld();
private:
    void loopDebounce();
    String _name;
	int _pin;
    int _state = HIGH;
    int _last_state = HIGH;
    unsigned long _last_debounce_time = 0;
    unsigned long _low_at_millis;
    bool _shortPressed = false;
    bool _hold_action_executed = false;
    void(*_funcShortPress)(void);
    bool(*_funcButtonHold)(void);
    bool _isButtonHeld = false;
};

#endif