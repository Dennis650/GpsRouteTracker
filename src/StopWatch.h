#ifndef stop_watch_h
#define stop_watch_h

#include "Arduino.h"

class StopWatch: public Printable
{
public:
  enum State      { RESET, RUNNING, STOPPED };

  void     start();
  void     stop();
  void     reset();
  // elapsed() made const as printTo expect a const object.
  uint32_t elapsed() const;    // replaced value()

  bool     isRunning() { return _state == StopWatch::RUNNING; };
  enum     State state() { return _state; };

  // PRINTABLE
  size_t printTo(Print& p) const;
  
private:
  enum State      _state;

  uint32_t _starttime;
  uint32_t _stoptime;
};

#endif

