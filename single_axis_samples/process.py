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
import hexdump as hx
# TODO: Take port as input
zmq_CH1 = "tcp://localhost:5555"
zmq_CH2 = "tcp://localhost:5556"
zmq_context_ch1 = zmq.Context()
zmq_context_ch2=zmq.Context()
zmq_sample_subscriber_ch1 = zmq_context_ch1.socket( zmq.SUB )
zmq_sample_subscriber_ch2 = zmq_context_ch2.socket( zmq.SUB )
zmq_sample_subscriber_ch2.setsockopt( zmq.CONFLATE, 1 )
zmq_sample_subscriber_ch1.setsockopt( zmq.CONFLATE, 1 )          # Keeps only latest message
zmq_sample_subscriber_ch2.connect( zmq_CH2 )
zmq_sample_subscriber_ch1.connect( zmq_CH1 )
zmq_sample_subscriber_ch1.setsockopt( zmq.SUBSCRIBE, "" )
zmq_sample_subscriber_ch2.setsockopt( zmq.SUBSCRIBE, "" )  # Listen to all messages on socket
def get_samples(): #collect abt 1000 samples for each channel for rate at 200k samples and 
                   #axis distance of 15 cm
   samples_count=0
   samples_x_tx_sensor=[]
   samples_x_rx_sensor=[]
   temp1=0
   temp2=0
   while(samples_count <800):
      temp1=zmq_sample_subscriber_ch1.recv()
      samples_x_tx_sensor.append(int(temp1,16))
      temp2=zmq_sample_subscriber_ch2.recv()
      samples_x_rx_sensor.append(int(temp2,16))
      samples_count=samples_count+1
      print samples_count
      #print samples_x_rx_sensor[samples_count],samples_x_tx_sensor[samples_count]
   return samples_x_rx_sensor,samples_x_tx_sensor



def get_tof(sam_TX,sam_RX):
   #print sam_RX
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
   if (tof!=0):
      pulse_speed=float(axis_distance/tof)
   else:	
      pulse_speed = 336
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

      #print samples_x_rx_sensor
      #print "hekko"
      #print samples_x_tx_sensor
      tof_x = get_tof(samples_x_tx_sensor,samples_x_rx_sensor)
      windspeed_x,direction_x=find_windspeed(temp,tof_x)
      print( "Temperature: " + str(temp) )
      print( "ToF_x: " + str(tof_x))
      print( "Wind speed=> x: " + str(windspeed_x)+" direction: "+str(direction_x))
      time.sleep(2) 



if __name__ == "__main__":
   run()
