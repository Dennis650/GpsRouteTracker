#include <Arduino.h>
#include <button.h>

bool const logDebug = false;

DebouncedButton::DebouncedButton(int pin, String name, void(*funcShortPress)(), bool(*funcButtonHold)()) {
    if (logDebug) {
        Serial.print("Constructor for ");
        Serial.print(name);
        Serial.print(" Pin: ");
        Serial.println(pin);
    }
	pinMode(pin, INPUT);
	_pin = pin;
    _name = name;
    _funcShortPress = funcShortPress;
    _funcButtonHold = funcButtonHold;
} 

void DebouncedButton::loopDebounce() {
  int stateRead = digitalRead(_pin);
  if (stateRead != _last_state) {
    _last_debounce_time = millis();
  }

  if ((millis() - _last_debounce_time) > debounceDelayLow) {
    if (stateRead != _state) {
      _state = stateRead;

      if (_state == LOW) {
        //Serial.println("Button " + _name + " gedrückt");
        _low_at_millis = millis();
      } else {
        //Serial.println("Button " + _name + " nicht gedrückt");

        long pressedMillis = millis() - _low_at_millis;

        if (pressedMillis < button_hold_threshold)
        {
            _shortPressed = true;
            //Serial.println(" + _name + "short pressed");
        }
        _low_at_millis = 0;
      }
    }
  }
  _last_state = stateRead;
}


void DebouncedButton::loop() {
    loopDebounce();

    if (_hold_action_executed==true) {
        _low_at_millis = 0;
        _last_state = HIGH;
        _hold_action_executed = false;
        if (logDebug) {
            Serial.println("Hold action " + _name + " wurde ausgeführt, setze zurück");
        }
    }

    if (_shortPressed)
    {
        if (logDebug) {
            Serial.println("Button " + _name + " pressed short");
        }
        //external function
        _funcShortPress();
        _shortPressed = false;
    }

    if (_low_at_millis != 0 && (millis() - _low_at_millis) >= button_hold_threshold) {
        int currentState = digitalRead(_pin);
        if (currentState==LOW) {
            _isButtonHeld = true;
            if (logDebug) {
                Serial.println("Button " + _name + " wird gehalten...");
            }

            if (!_hold_action_executed) {
                //external function
                _hold_action_executed = _funcButtonHold();
            }
        } else {
            if (logDebug) {
                Serial.println("Button " + _name + " wird gar nicht mehr gedrück!!");
            }
            _hold_action_executed = true;
            _isButtonHeld = false;
        }
    }
}

bool DebouncedButton::isButtonHeld() {
    return _isButtonHeld;
}