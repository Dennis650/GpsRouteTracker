#include <Arduino.h>
#include <SdFat.h>
#include <TinyGPS++.h>
#include <TimeLib.h>

static bool isSummertimeBool = false;

boolean isSummertime() {
    return isSummertimeBool;
}

boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
    if (month < 3 || month > 10)
        return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
    if (month > 3 && month < 10)
        return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
    if ((month == 3 && ((hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7)))) || (month == 10 && ((hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7)))))
    {
        return true;
    }
    else
    {
        return false;
    }
}

time_t gpsTimeToArduinoTime(TinyGPSDate date, TinyGPSTime time)
{
    if (date.isValid() && date.year() != 2000)
    {
        if (day() != date.day() || (year()) != date.year())
        {
            //Serial.print("isSummertime?");
            isSummertimeBool = summertime_EU(date.year(), date.month(), date.day(), time.hour(), 0);
            //Serial.print(" -> ");
            //Serial.print(isSummertimeBool);
            //Serial.println();
        }
    }

    tmElements_t tm;
    tm.Year = date.year() - 1970; // years since 1970
    tm.Month = date.month();
    tm.Day = date.day();
    tm.Hour = time.hour();
    tm.Minute = time.minute();
    tm.Second = time.second();
    time_t time4Arduino = makeTime(tm); // seconds since 1970

    setTime(time4Arduino);

    if (isSummertime())
    {
        adjustTime(2 * 3600);
    }
    else
    {
        adjustTime(1 * 3600);
    }

    return time4Arduino; // UTC
}


char*  getFormattedLapTime(uint32_t time, char* buffer) {
  int millisec  = time % 1000;
  int tseconds = time / 1000;
  int tminutes = tseconds / 60;
  int seconds = tseconds % 60;

  sprintf(buffer,"%02u:%02u:%03u", tminutes, seconds, millisec); 

  //char str[16];  

  return buffer;
}