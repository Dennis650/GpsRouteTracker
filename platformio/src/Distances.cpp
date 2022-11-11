#include <Arduino.h>
#include <Distances.h>
#include <TrackerConfig.h>
#include <GpsData.h>
#include <global_vars.h>

Distances::Distances()
{
}

void Distances::init()
{
    _fuelMax = ((100.0 / trackerConfig.durchschnittsverbrauch) * (trackerConfig.tankinhalt - trackerConfig.tankreserve)) * 1000.0;
    _fuelMaxWithReserve = ((100.0 / trackerConfig.durchschnittsverbrauch) * (trackerConfig.tankinhalt)) * 1000.0;
}

void Distances::addDistance(const unsigned long distanceKM)
{
    _odo += distanceKM;
    _trip1 += distanceKM;
    _trip2 += distanceKM;
    _fuel += distanceKM;
}

unsigned long Distances::getOdo()
{
    return _odo;
}

double Distances::getOdoKM()
{
    return _odo / 1000.0;
}

unsigned long Distances::getTrip1()
{
    return _trip1;
}

double Distances::getTrip1KM()
{
    return _trip1 / 1000.0;
}

unsigned long Distances::getTrip2()
{
    return _trip2;
}

double Distances::getTrip2KM()
{
    return _trip2 / 1000.0;
}

unsigned long Distances::getFuel()
{
    return _fuel;
}

double Distances::getFuelKM()
{
    return _fuel / 1000.0;
}

unsigned long Distances::getFuelMax()
{
    return _fuelMax;
}

double Distances::getFuelMaxKM()
{
    return _fuelMax / 1000.0;
}

unsigned long Distances::getFuelRemaining()
{
    return _fuelMax - _fuel;
}

double Distances::getFuelRemainingKM()
{
    if (_fuel>_fuelMax) 
    {
        return 0;
    }
    return (_fuelMax - _fuel) / 1000.0;
}

double Distances::getFuelRemainingKMWithReserve()
{
    if (_fuel>_fuelMaxWithReserve) 
    {
        return 0;
    }
    return (_fuelMaxWithReserve - _fuel) / 1000.0;
}

void Distances::setOdo(unsigned long distance)
{
    _odo = distance;
}

void Distances::setTrip1(unsigned long distance)
{
    _trip1 = distance;
}
void Distances::setTrip2(unsigned long distance)
{
    _trip2 = distance;
}
void Distances::setFuel(unsigned long distance)
{
    _fuel = distance;
}
void Distances::setFuelMax(unsigned long distance)
{
    _fuelMax = distance;
}

unsigned long Distances::getTrip1StartMillis() {
    return _trip1_start_millis;
}

unsigned long Distances::getTrip2StartMillis() {
    return _trip2_start_millis;
}

void Distances::setTrip1StartMillis(unsigned long millis) {
    _trip1_start_millis = millis;
}

void Distances::setTrip2StartMillis(unsigned long millis) {
    _trip2_start_millis = millis;
}

double Distances::getTrip1OverageSpeed() {
    /*
    Serial.println("---------------------------");
    Serial.println(millis());
    Serial.println(_trip1_start_millis);
    Serial.println(getTrip1KM());
    Serial.println(((millis() - _trip1_start_millis)/1000.0/60.0/60.0));
    Serial.println(getTrip1KM() / ((millis() - _trip1_start_millis)/1000.0/60.0/60.0));
    */

    return getTrip1KM() / ((millis() - _trip1_start_millis)/1000.0/60.0/60.0);
}

double Distances::getTrip2OverageSpeed() {
    return getTrip2KM() / ((millis() - _trip2_start_millis)/1000.0/60.0/60.0);
}