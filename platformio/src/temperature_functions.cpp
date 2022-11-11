#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

//OneWire
OneWire oneWire(25); //
DallasTemperature sensors(&oneWire);
DeviceAddress temperatureDeviceAddress;
static unsigned long lastTemperatureRequest = 0;

float temperatur = -99;

void temperatureSensorRequest()
{
    unsigned long currentMillis = millis();

    if (currentMillis - lastTemperatureRequest > 5000)
    {
        //Serial.println("temperatureSensorRequest...");
        sensors.requestTemperatures();
        lastTemperatureRequest = currentMillis;

        float readTemp = sensors.getTempCByIndex(0);
        //Serial.print("test temperatureSensorRequest: ");
        //Serial.println(readTemp);

        if (readTemp!=127.0) {
            temperatur = readTemp;
        }
    }
}

float getTemperatureSensorValue()
{
    return temperatur;
}

void temperatureLoop() {
    temperatureSensorRequest();
}

void temperatureSetup()
{
    Serial.println("temperatureSetup");

    sensors.begin();
    sensors.getAddress(temperatureDeviceAddress, 0);
    sensors.setResolution(temperatureDeviceAddress, 11);
    sensors.setWaitForConversion(true);
}