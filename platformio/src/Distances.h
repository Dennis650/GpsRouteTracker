#ifndef Distances_h
#define Distances_h

#include <Arduino.h>

class Distances
{
  public:
    Distances();
    void init();
    unsigned long getOdo();
    unsigned long getTrip1();
    unsigned long getTrip1StartMillis();
    unsigned long getTrip2();
    unsigned long getTrip2StartMillis();
    unsigned long getFuel();
    unsigned long getFuelMax();
    unsigned long getFuelRemaining();
    
    void setOdo(unsigned long distance);
    void setTrip1(unsigned long distance);
    void setTrip1StartMillis(unsigned long millis);
    void setTrip2(unsigned long distance);
    void setTrip2StartMillis(unsigned long millis);
    void setFuel(unsigned long distance);
    void setFuelMax(unsigned long distance);
    void setFuelRemaining(unsigned long distance);

    double getOdoKM();
    double getTrip1KM();
    double getTrip1OverageSpeed();
    double getTrip2KM();
    double getTrip2OverageSpeed();
    double getFuelKM();
    double getFuelMaxKM();
    double getFuelRemainingKM();
    double getFuelRemainingKMWithReserve();
    void addDistance(const unsigned long distanceMeters);
  private:
    unsigned long _odo;
    unsigned long _trip1;
    unsigned long _trip1_start_millis;
    unsigned long _trip2;
    unsigned long _trip2_start_millis;
    unsigned long _fuel;
    unsigned long _fuelMax;
    unsigned long _fuelMaxWithReserve;
};

#endif