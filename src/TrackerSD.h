#ifndef TrackerSD_h
#define TrackerSD_h

#include <Arduino.h>
#include <SdFat.h>


class TrackerSD
{
  public:
    TrackerSD(int sd_chipSelect);
    void begin();
    SdFat getSd();
  private:
    int _sd_chipSelect;
    SdFat _sd;
    File32 _file;
};

TrackerSD *getTrackerSD();

#endif