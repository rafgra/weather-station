#define ESP8266_DEBUG

#include "DallasTemperature.h"
#include "OneWire.h"
#include "Wire.h"
#include "BH1750.h"
#include "LPS.h"
#include "ESP8266.h"
#include "DHT.h"
#include "LiquidCrystal.h"
#include "WeatherStationCore.h"
#include "Metro.h"

#define WIFI_SSID_NAME "test"
#define WIFI_PASSWORD "test"

#define DST_IP "0.0.0.0"
#define DST_PORT 6000
#define SEND_INTERVAL 10000
#define DISPLAY_INTERVAL 1000
#define SEND_RETRY 3


ESP8266 wifi(WIFI_MODE_STA, 115200);
LiquidCrystal lcd(29, 28, 27, 26, 25, 24);

Metro wifiSendTimer = Metro(SEND_INTERVAL);
Metro displayTimer = Metro(DISPLAY_INTERVAL);

int _retryCounter = 0;
WeatherStationCore station;

void displayMessage()
{
    if (displayTimer.check() == 1)
    {
        lcd.setCursor(0, 0);
        lcd.print(station.display(0));
        lcd.setCursor(0, 1);
        lcd.print(station.display(1));
    }
}

void sendJSON()
{
    if (wifiSendTimer.check() == 1)
    {
        _retryCounter++;
        if (wifi.startClient(DST_IP, DST_PORT))
        {
            String json = station.getJSON();
            wifi.send(json);
            _retryCounter = 0;
        }

    }
}


void wifiInit()
{

   wifi.initializeWifi(NULL, NULL);
   wifi.connectWifi(WIFI_SSID_NAME, WIFI_PASSWORD);
}

void stationInit()
{
    station.init();
    station.run();
}

void lcdInit()
{
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Weather Station");
    lcd.setCursor(0, 1);
    lcd.print("Initialize...");
}

void setup()
{
    lcdInit();
    stationInit();
    wifiInit();

}

void loop()
{
    station.run();
    sendJSON();
    displayMessage();

    if (_retryCounter >= SEND_RETRY)
    {
        _retryCounter = 0;
        wifiInit();
    }

}

