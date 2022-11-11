#include <Arduino.h>
#include <TrackerSD.h>
#include <TimeLib.h>

TrackerSD *trackerSD;

TrackerSD *getTrackerSD()
{
    if (!trackerSD) {
        trackerSD = new TrackerSD(5);
    }
    return trackerSD;
}

TrackerSD::TrackerSD(int sd_chipSelect) {
    //Serial.println("Constructor TrackerSD");
    _sd_chipSelect = sd_chipSelect;
}


void dateTimeSdFat(uint16_t *date, uint16_t *time)
{
    // return date using FAT_DATE macro to format fields
    *date = FAT_DATE(year(), month(), day());

    // return time using FAT_TIME macro to format fields
    *time = FAT_TIME(hour(), minute(), second());
}

void TrackerSD::begin() 
{
    //Serial.println("TrackerSD::begin");

    //Nicht über 50 MHz
    if (!_sd.begin(_sd_chipSelect, SD_SCK_MHZ(50 / 8)))
    {
        _sd.initErrorPrint();
    }
    else
    {
        //Serial.println("SD-Card erfolgreich eingebunden!");
    }

    SdFile::dateTimeCallback(dateTimeSdFat);
}

SdFat TrackerSD::getSd()
{
    return _sd;
}

