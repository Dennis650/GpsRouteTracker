#ifndef lap_h
#define lap_h

#include "Arduino.h"
#include "StopWatch.h"

class Lap
{
public:
  void        setStartFinishLatitude(float latitude);
  void        setStartFinishLongitude(float longitude);
  float       getStartFinishLatitude();
  float       getStartFinishLongitude();
  StopWatch * getStopWatch();
  bool        executeCurrentDistanceToStartFinish(double distanceToLapStartFinish);
  uint32_t    getFastestLap();
  uint32_t    getLastLap();
  void        setFastestLap(uint32_t time);
  void        setLastLap(uint32_t time);
  bool        isPositionInStartFinishZone();
  
private:
  float _start_finish_latitude = 0; 
  float _start_finish_longitude = 0;
  bool _positionInStartFinishZone = false;
  StopWatch _stopWatch;
  uint32_t _fastestLap = 0;
  uint32_t _lastLap = 0;
};

#endif

