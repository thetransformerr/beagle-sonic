#! /usr/bin/python

#
# FUSION.PY
#
# Description: Sensor fusion for the sonic anemometer
#
# (C) 2016 Visaoni
# Licensed under the MIT License.
#

# TODO: Figure out if DHT or BMP is better for temp

import zmq
import Adafruit_DHT
import Adafruit_BMP.BMP085 as Adafruit_BMP085
import math
import time

# TODO: Take port as input
zmq_tof_host = "tcp://localhost:5555"


zmq_context = zmq.Context()
zmq_tof_subscriber = zmq_context.socket( zmq.SUB )
zmq_tof_subscriber.setsockopt( zmq.CONFLATE, 1 )           # Keeps only latest message
zmq_tof_subscriber.connect( zmq_tof_host )
zmq_tof_subscriber.setsockopt( zmq.SUBSCRIBE, "" )  # Listen to all messages on socket

# TODO: Deal with the fact there are actually two axes
def get_tof():
   return float( zmq_tof_subscriber.recv() ) / 10**9  # Convert ns to s


DHT_sensor = Adafruit_DHT.AM2302
DHT_pin = "UART2_RXD"

# Adafruit_DHT does offer read_retry but has no way to specify infinite retries
# Also returns an error code on failure rather than an exception, hence this construct
def get_rel_humidity_temp():
   result = (None, None)
   while result == (None, None):
      result = Adafruit_DHT.read( DHT_sensor, DHT_pin )
      time.sleep(2)
   return result


BMP_busnum = 2
BMP_sensor = Adafruit_BMP085.BMP085( busnum = BMP_busnum, mode = Adafruit_BMP085.BMP085_ULTRAHIGHRES )

def get_temp():
   return BMP_sensor.read_temperature()

def get_pressure():
   return BMP_sensor.read_pressure()


# TODO: Take as input, or better yet get from configuration process
#     If windspeed is assumed to be 0, distance can be calculated from tof
axis_distance = 0.5

def find_windspeed( rel_humidity, temp, pressure, tof ):
   pulse_speed = axis_distance / tof
   return pulse_speed - est_speed_of_sound( rel_humidity, temp, pressure )


# Based on Bohn, Dennis A. "Environmental effects on the speed of sound." Journal of the Audio Engineering Society 36.4 (1988): 223-231.
def est_speed_of_sound( rel_humidity, temp, pressure ):
   R = 8.3144598

   temp += 273.15    # Celsius to Kelvin
   h = rel_humidity * 0.01 * water_vapor_pressure( temp ) / pressure
   gamma = (7 + h) / (5 + h)
   M = (29 - (11 * h)) / 10**3

   return math.sqrt( (gamma * R * temp) / M )

# Based on the Antonine approximation
# TODO: This is only good down to 1C
def water_vapor_pressure( temp ):
   A = 8.07131
   B = 1730.63
   C = 233.426

   temp -= 273.15          # Kelvin to Celsius

   P = 10 ** (A - (B / (C + temp) ))   # In torrs
   return P * 133.322




# TODO: Add more frequent updates (IE avoid having to wait on the DHT)
# Most basic setup possible
# Loop through and grab all sensor readings then analyze
def run():
   while True:
      rel_humidity, temp = get_rel_humidity_temp()
      pressure = get_pressure()
      tof = get_tof()

      print( "Humidity:    " + str(rel_humidity) )
      print( "Temperature: " + str(temp) )
      print( "ToF:         " + str(tof) )
      print( "Wind speed:  " + str(find_windspeed( rel_humidity, temp, pressure, tof )) )

      time.sleep(2) # Wait for DHT



if __name__ == "__main__":
   run()