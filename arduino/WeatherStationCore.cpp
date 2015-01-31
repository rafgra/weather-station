
#include "WeatherStationCore.h"

WeatherStationCore::WeatherStationCore()
{
    _timer = Metro(5000);


    //DHT21
    _dht21Temperature = 0.0f;
    _humidity = 0.0f;

    //DS18B20
    _ds18b20InsideTemperature = 0.0f;
    _ds18b20OutsideTemperature = 0.0f;

    //LPS
    _lpsTemperature = 0.0f;
    _pressure = 0.0f;

    //BH1750
    _illuminance = 0.0f;

    //Calculated
    _dewPoint = 0.0f;
    _avgInsideTemperature = 0.0f;
    _avgOutsideTemperature = 0.0f;
}

InitStatus WeatherStationCore::init(void)
{
    if (!initBH1750())
        return BH1750_FAIL;
    if (!initDS18B20())
        return DS18B20_FAIL;
    if (!initDHT21())
        return DHT21_FAIL;
    if (!initLPS())
        return LPS_FAIL;

    return NO_ERR;
}

void WeatherStationCore::run(void)
{
    if (_timer.check() == 1)
    {
#ifdef STATION_DEBUG
        debug("Refresh!");
#endif
        readFromDS18B20();
        readFromLPS();
        readFromBH1750();
        readFromDHT21();
        _avgInsideTemperature = (_ds18b20InsideTemperature + _lpsTemperature) / 2.0f;
        _avgOutsideTemperature = (_ds18b20OutsideTemperature + _dht21Temperature) / 2.0f;
        _dewPoint = calculateDewPiont(_ds18b20OutsideTemperature, _humidity);
    }
}

void WeatherStationCore::setInterval(long interval)
{
    if (interval > 1000) _timer.interval(interval);
}

bool WeatherStationCore::initBH1750(void)
{
#ifdef STATION_DEBUG
    debug("Init BH1750");
#endif
    lightSensor.begin(BH1750_CONTINUOUS_HIGH_RES_MODE_2);
#ifdef STATION_DEBUG
    debug("BH1750 -> OK");
#endif
    readFromBH1750();

    return true;
}

bool WeatherStationCore::initDS18B20(void)
{
#ifdef STATION_DEBUG
    debug("Init DS18B20");
#endif
    sensors.begin();

#ifdef STATION_DEBUG
    debug("Found " + String(sensors.getDeviceCount()) + " devices.");
#endif

    if (sensors.getAddress(insideThermometer, 0))
    {
        sensors.setResolution(insideThermometer, DS18B20_PRECISION);
    }
#ifdef STATION_DEBUG
    else
    {
        debug("Unable to find address for Device DS18B20 Inside");
    }
#endif

    if (sensors.getAddress(outsideThermometer, 1))
    {
        sensors.setResolution(outsideThermometer, DS18B20_PRECISION);
    }
#ifdef STATION_DEBUG
    else
    {
        debug("Unable to find address for Device DS18B20 Outside");
    }
#endif


#ifdef STATION_DEBUG
    debug("Resolution: " + String(sensors.getResolution(insideThermometer)));
#endif

#ifdef STATION_DEBUG
    debug("DS18B20 -> OK");
#endif

    readFromDS18B20();
    return true;
}

bool WeatherStationCore::initLPS(void)
{
#ifdef STATION_DEBUG
    debug("Init LPS");
#endif
    Wire.begin();

    if (!ps.init())
    {
#ifdef STATION_DEBUG
        debug("Failed to autodetect pressure sensor!");
#endif
        return false;
    }

    ps.enableDefault();
#ifdef STATION_DEBUG
    debug("LPS -> OK");
#endif
    readFromLPS();
    return true;
}

bool WeatherStationCore::initDHT21(void)
{
#ifdef STATION_DEBUG
    debug("Init DHT21");
#endif
    dht.begin();

#ifdef STATION_DEBUG
    debug("DHT21 -> OK");
#endif
    readFromDHT21();
    return true;
}

void WeatherStationCore::readFromLPS(void)
{
    float pressure = ps.readPressureMillibars();
    float temperature = ps.readTemperatureC();

    if (!isnan(pressure))
        _pressure = pressure;
    if (!isnan(temperature))
        _lpsTemperature = temperature;

#ifdef STATION_DEBUG
    String msg = "LPS -> ";
    msg += "Temperature: ";
    msg += floatToString(temperature);
    msg += " *C";
    msg += "\t Pressure: ";
    msg += floatToString(pressure);
    msg += " hPa";
    debug(msg);
#endif
}

void WeatherStationCore::readFromDS18B20(void)
{
    sensors.requestTemperatures();
    float temperatureInside = sensors.getTempC(insideThermometer);
    float temperatureOutside = sensors.getTempC(outsideThermometer);

    if (!isnan(temperatureInside))
        _ds18b20InsideTemperature = temperatureInside;
    if (!isnan(temperatureOutside))
        _ds18b20OutsideTemperature = temperatureOutside;

#ifdef STATION_DEBUG
    String msg = "DS18B20 -> ";
    msg += "Temperature inside: ";
    msg += floatToString(temperatureInside);
    msg += " *C";
    msg += "\tTemperature outside: ";
    msg += floatToString(outsideThermometer);
    msg += " *C";
    debug(msg);
#endif
}

void WeatherStationCore::readFromBH1750(void)
{
    float lux = lightSensor.readLightLevel();
    if (!isnan(lux))
        _illuminance = lux;
#ifdef STATION_DEBUG
    String msg = "BH1750 -> ";
    msg += "Illuminance: ";
    msg += floatToString(lux);
    msg += " lux";
    debug(msg);
#endif
}

void WeatherStationCore::readFromDHT21(void)
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (!isnan(humidity))
        _humidity = humidity;
    if (!isnan(temperature))
        _dht21Temperature = temperature;
#ifdef STATION_DEBUG
    String msg = "DHT21 -> ";
    msg += "Temperature: ";
    msg += floatToString(temperature);
    msg += " *C";
    msg += "\t Humidity: ";
    msg += floatToString(humidity);
    msg += "%";
    debug(msg);
#endif
}

float WeatherStationCore::calculateDewPiont(float celsius, float humidity)
{
    double A0 = 373.15 / (273.15 + celsius);
    double SUM = -7.90298 * (A0 - 1);
    SUM += 5.02808 * log10(A0);
    SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1) ;
    SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1) ;
    SUM += log10(1013.246);
    double VP = pow(10, SUM - 3) * humidity;
    double T = log(VP / 0.61078); // temp var
    return (241.88 * T) / (17.558 - T);

    /*
    float a = 17.271f;
    float b = 237.7f;
    float temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
    float Td = (b * temp) / (a - temp);
    return Td;
    */
}

String WeatherStationCore::getJSON(void)
{
    String json = "   {";
    json += "\"outside\":{";
    json += "\"dht21\":" + floatToString(_dht21Temperature) + ",";
    json += "\"ds18b20\":" + floatToString(_ds18b20OutsideTemperature) + ",";
    json += "\"avg\":" + floatToString(_avgOutsideTemperature) + "},";

    json += "\"inside\":{";
    json += "\"ds18b20\":" + floatToString(_ds18b20InsideTemperature) + ",";
    json += "\"lps\":" + floatToString(_lpsTemperature) + ",";
    json += "\"avg\":" + floatToString(_avgInsideTemperature) + "},";

    json += "\"humidity\":" + floatToString(_humidity) + ",";
    json += "\"pressure\":" + floatToString(_pressure) + ",";
    json += "\"illuminance\":" + floatToString(_illuminance) + ",";
    json += "\"dew_point\":" + floatToString(_dewPoint);
    json += "}   ";

#ifdef STATION_DEBUG
    debug(json);
#endif
    return json;
}

String WeatherStationCore::display(int line)
{
    String msg = "";
    if (line == 0)
    {
        msg += "I:" + floatToString(_avgInsideTemperature, 5, 1) ;
        msg += " O:" + floatToString(_ds18b20OutsideTemperature, 5, 1);
    }
    else
    {
        msg += floatToString(_pressure, 7, 2) + "hPa ";
        msg += floatToString(_humidity, 3, 0) + "%";
    }
    return msg;
}