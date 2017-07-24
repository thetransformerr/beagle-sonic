#! /usr/bin/python

#
# FUSION.PY
#
# Description: Sensor fusion for the sonic anemometer
#


import zmq
import math
import time
import numpy as np

# TODO: Take port as input
zmq_tof_host = "tcp://localhost:5555"


zmq_context = zmq.Context()
zmq_sample_subscriber = zmq_context.socket( zmq.SUB )
zmq_sample_subscriber.setsockopt( zmq.CONFLATE, 1 )           # Keeps only latest message
zmq_sample_subscriber.connect( zmq_tof_host )
zmq_sample_subscriber.setsockopt( zmq.SUBSCRIBE, "" )  # Listen to all messages on socket

def get_samples(): #collect abt 1000 samples for each channel for rate at 200k samples and 
                   #axis distance of 15 cm
   samples_count=0
   samples_x_tx_sensor=[]
   samples_x_rx_sensor=[]
   while(samples_count <2400):
      s=samples_count%2
      if(s==0):
         samples_x_tx_sensor.append(zmq_sample_subscriber.recv())
      elif(s==1):
         samples_x_rx_sensor.append(zmq_sample_subscriber.recv())
      samples_count=samples_count+1
   return samples_x_rx_sensor,samples_x_tx_sensor



def get_tof(sam_TX,sam_RX):
   mean=np.mean(sam_RX)
   normalised_tx=sam_TX-mean
   normalised_rx=sam_RX-mean
   correlate_np=np.correlate(normalised_tx,normalised_rx,'same')
   tof=np.argmax(correlate_np)-(len(normalised_tx)/2)
   return tof

def get_temp():
   temp=25
   return temp # to implement working for sampling analog values from sensor like lm-35


# TODO: Take as input, or better yet get from configuration process
#     If windspeed is assumed to be 0, distance can be calculated from tof
axis_distance = 0.15

#Based on https://en.wikipedia.org/wiki/Speed_of_sound#Practical_formula_for_dry_air
def find_windspeed( temp,tof ):
   pulse_speed = float(axis_distance / tof)
   temp += 273.15    # Celsius to Kelvin
   sound_speed=float(20.05*math.sqrt(temp))
   speed=pulse_speed-sound_speed
   if (speed >=0):
      direction="From tx to rx"
   else:
      direction="From rx to tx"
   return speed,direction

def run():
   while True:
      temp = get_temp()
      samples_x_rx_sensor,samples_x_tx_sensor=get_samples()
      tof_x = get_tof(samples_x_tx_sensor,samples_x_rx_sensor)
      windspeed_x,direction_x=find_windspeed(tof_x,temp)
      print( "Temperature: " + str(temp) )
      print( "ToF_x: " + str(tof_x))
      print( "Wind speed=> x: " + str(windspeed_x)+" direction: "+str(direction_x))
      time.sleep(2) 



if __name__ == "__main__":
   run()
