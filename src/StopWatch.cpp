#include "Arduino.h"
#include "StopWatch.h"

void StopWatch::reset()
{
  _state      = StopWatch::RESET;
  _starttime  = _stoptime = 0;
}


void StopWatch::start()
{
  if (_state == StopWatch::RESET || _state == StopWatch::STOPPED)
  {
    _state = StopWatch::RUNNING;
    uint32_t t = millis();
    _starttime += t - _stoptime;
    _stoptime = t;
  }
}


// made const as printTo expect a const object.
uint32_t StopWatch::elapsed() const
{
  if (_state == StopWatch::RUNNING)
  {
    return millis() - _starttime;
  }
  return _stoptime - _starttime;
}


void StopWatch::stop()
{
  if (_state == StopWatch::RUNNING)
  {
    _stoptime = millis();
    _state = StopWatch::STOPPED;
  }
}

// PRINTABLE
size_t StopWatch::printTo(Print& p) const
{
  uint32_t ti = StopWatch::elapsed();
  return p.print(ti);
}
