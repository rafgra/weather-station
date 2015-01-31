#include <stdlib.h>
#include <Arduino.h>
#include <DHT.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <BH1750.h>
#include <LPS.h>
#include <Metro.h>

#ifndef WEATHER_STATION_CORE_H
#define WEATHER_STATION_CORE_H

#define DS18B20_PIN 23
#define DS18B20_PRECISION 12

#define DHTPIN 22
#define DHTTYPE DHT21

//#define STATION_DEBUG

#ifdef STATION_DEBUG
#define dbg Serial
#endif

static LPS ps;
static BH1750 lightSensor;
static DHT dht(DHTPIN, DHTTYPE);
static OneWire oneWire(DS18B20_PIN);
static DallasTemperature sensors(&oneWire);
static DeviceAddress insideThermometer;
static DeviceAddress outsideThermometer;

enum InitStatus
{
    BH1750_FAIL = 0,
    DHT21_FAIL,
    LPS_FAIL,
    DS18B20_FAIL,
    NO_ERR
};

enum Sensor
{
    DS18B20 = 0,
    LPS,
    DHT,
    AVG
};

class WeatherStationCore
{
public:
    WeatherStationCore();
    InitStatus init();
    void run();
    void setInterval(long interval);

    float getOutsideTemperature(Sensor sensor = AVG)
    {
        switch (sensor)
        {
        case DS18B20:
            return _ds18b20OutsideTemperature;
        case DHT:
            return _dht21Temperature;
        case AVG:
            return _avgOutsideTemperature;
        default:
            return 0;
        }
    }
    float getInsideTemperature(Sensor sensor = AVG)
    {
        switch (sensor)
        {
        case DS18B20:
            return _ds18b20InsideTemperature;
        case LPS:
            return _lpsTemperature;
        case AVG:
            return _avgInsideTemperature;
        default:
            return 0;
        }
    }
    float getPressure()
    {
        return _pressure;
    }
    float getIlluminance()
    {
        return _illuminance;
    }
    float getHumidity()
    {
        return _humidity;
    }
    float getDewPoint()
    {
        return _dewPoint;
    }

    String display(int line);

    String getJSON(void);

#ifdef STATION_DEBUG
    void debug(String msg)
    {
        if (dbg)
            dbg.println(msg);
    }
#endif

private:

    /** VARIABLES **/

    //DHT21
    float _dht21Temperature;
    float _humidity;

    //DS18B20
    float _ds18b20InsideTemperature;
    float _ds18b20OutsideTemperature;

    //LPS
    float _lpsTemperature;
    float _pressure;

    //BH1750
    float _illuminance;

    Metro _timer;
    float _dewPoint;
    float _avgInsideTemperature;
    float _avgOutsideTemperature;


    /** INIT SENSORS **/
    bool initBH1750(void);
    bool initDS18B20(void);
    bool initLPS(void);
    bool initDHT21(void);

    /** READ FUNCTIONS **/
    void readFromLPS(void);
    void readFromDS18B20(void);
    void readFromBH1750(void);
    void readFromDHT21(void);

    float calculateDewPiont(float celsius, float humidity);

    String floatToString(float data, int space = 1, int precision = 2)
    {
        if (space > 10) space = 9;

        char buffer[16] = {0};
        return dtostrf(data, space, precision, buffer);
    }

};


#endif