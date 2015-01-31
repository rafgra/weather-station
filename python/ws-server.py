#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os,signal,sys
import SocketServer
import json
import datetime
from storm.locals import *

def set_exit_handler(func):
    signal.signal(signal.SIGTERM, func)
def on_exit(sig, func=None):
    print "exit handler triggered"
    sys.exit(1)

if __name__ == "__main__":
    set_exit_handler(on_exit)

db = create_database('mysql://dbuser:dbpasswd@localhost:3306/weather_station')
store = Store(db)

def round_seconds(t): # t is a datetime object
    return t - datetime.timedelta(seconds=t.second-round(t.second, -1))



class Record(object):
   __storm_table__='records'
   id = Int(primary=True)
   date = DateTime()
   temperature = Float()
   humidity = Float()
   pressure = Float()
   illuminance = Float()
   dewpoint = Float()

class Log(object):
    __storm_table__='logs'
    id = Int(primary=True)
    date = DateTime()
    message = Unicode()


class WeatherStationTCPServer(SocketServer.ThreadingTCPServer):
    allow_reuse_address = True

class WeatherStationTCPServerHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        try:
            dateTime = round_seconds(datetime.datetime.now()).replace(microsecond=0)
            data = self.request.recv(1024).decode('UTF-8').strip()
            log = Log()
            log.date = dateTime
            log.message = data
            store.add(log)
            store.commit()
            data = json.loads(data)
            rec = Record()
            rec.date = dateTime
            rec.temperature = data['outside']['ds18b20']
            rec.humidity = data['humidity']
            rec.pressure = data['pressure']
            rec.illuminance = data['illuminance']
            rec.dewpoint = data['dew_point']
            store.add(rec)
            store.commit()
            #print(data)
            #temperature = '{:+.2f}°C'.format(rec.temperature)
            #humidity = '{:.2f}%'.format(rec.humidity)
            #pressure = '{:.2f} hPa'.format(rec.pressure)
            #illuminance = '{:.2f} lux'.format(rec.illuminance)
            #dew_point = '{:+.2f}°C'.format(rec.dewpoint)

            #print "---------[" ,dateTime, "]---------"
            #print "Temperature:\t" , temperature
            #print "Humidity:\t" , humidity
            #print "Pressure:\t" , pressure
            #print "Illuminance:\t" , illuminance
            #print "Dew piont:\t", dew_point

        except Exception as e:
            print"Err! ", e
        except KeyboardInterrupt:
            raise

try:
   server = WeatherStationTCPServer(('', 6000), WeatherStationTCPServerHandler)
   server.serve_forever()
except (KeyboardInterrupt, SystemExit):
   print ('\nkeyboardinterrupt caught (again)')
   print ('\n...Program Stopped Manually!')
   raise
