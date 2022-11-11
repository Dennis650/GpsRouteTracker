#include "Arduino.h"
#include "StopWatch.h"
#include "Lap.h"
#include <ScreenLapTime.h>

void Lap::setStartFinishLatitude(float latitude)
{
  _start_finish_latitude = latitude;
}

void Lap::setStartFinishLongitude(float longitude)
{
  _start_finish_longitude = longitude;
}

float Lap::getStartFinishLatitude()
{
  return _start_finish_latitude;
}

float Lap::getStartFinishLongitude()
{
  return _start_finish_longitude;
}

StopWatch * Lap::getStopWatch()
{
  return & _stopWatch;
}

bool Lap::executeCurrentDistanceToStartFinish(double distanceToLapStartFinish)
{
  if (distanceToLapStartFinish < 5) {
    uint32_t lapTime = _stopWatch.elapsed();

    bool _previousPositionInStartFinishZone = _positionInStartFinishZone;
    _positionInStartFinishZone = true;

    //Execute only once, when startFinishZone ist entered
    if (_positionInStartFinishZone == true && _previousPositionInStartFinishZone == false) {
      if (_stopWatch.isRunning()) {
        _stopWatch.reset();
        _stopWatch.start();
        _lastLap = lapTime;
        if (_fastestLap > lapTime || _fastestLap == 0) {
          _fastestLap = lapTime;
        }
        return true;
      } else {
        if (getCurrentScreenName() == SCREEN_LAP_TIME) {
          _stopWatch.reset();
          _stopWatch.start();
        }
      }
    }
  } else {
    _positionInStartFinishZone = false;
  }

  //Serial.print("Is in start /finish zone: ");
  //Serial.println(_positionInStartFinishZone);

  return false;
}

bool Lap::isPositionInStartFinishZone() 
{
  return _positionInStartFinishZone;
}

uint32_t Lap::getFastestLap() 
{
  return _fastestLap;
}

uint32_t Lap::getLastLap()
{
  return _lastLap;
}

void Lap::setFastestLap(uint32_t time)
{
  _fastestLap = time;
}
void Lap::setLastLap(uint32_t time)
{
  _lastLap = time;
}

