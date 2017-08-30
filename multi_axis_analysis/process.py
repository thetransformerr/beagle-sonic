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
zmq_sample_x_host = "tcp://localhost:5555"
zmq_sample_y_host = "tcp://localhost:5556"
zmq_temp_LM35_host ="tcp://localhost:5557"

zmq_context_x = zmq.Context()
zmq_sample_subscriber_x = zmq_context_x.socket( zmq.SUB )
zmq_sample_subscriber_x.setsockopt( zmq.CONFLATE, 1 )           # Keeps only latest message
zmq_sample_subscriber_x.connect( zmq_sample_x_host )
zmq_sample_subscriber_x.setsockopt( zmq.SUBSCRIBE, "" )  # Listen to all messages on socket


zmq_context_y = zmq.Context()
zmq_sample_subscriber_y = zmq_context_y.socket( zmq.SUB )
zmq_sample_subscriber_y.setsockopt( zmq.CONFLATE, 1 )           # Keeps only latest message
zmq_sample_subscriber_y.connect( zmq_sample_y_host )
zmq_sample_subscriber_y.setsockopt( zmq.SUBSCRIBE, "" )  # Listen to all messages on socket


zmq_context_temp = zmq.Context()
zmq_sample_subscriber_temp = zmq_context_temp.socket( zmq.SUB )
zmq_sample_subscriber_temp.setsockopt( zmq.CONFLATE, 1 )           # Keeps only latest message
zmq_sample_subscriber_temp.connect( zmq_temp_LM35_host )
zmq_sample_subscriber_temp.setsockopt( zmq.SUBSCRIBE, "" )  # Listen to all messages on socket

def get_samples(zmq_sample_subscriber_XorY): #collect abt 2400 samples for each channel for rate at 200k samples and 
                   #axis distance of 15 cm
   samples_count=0
   samples_tx_sensor=[]
   samples_rx_sensor=[]
   temp=0
   while(samples_count <5000):
      temp=zmq_sample_subscriber_XorY
      temp=temp[1:]
      temp=hx.dump(temp1,size=4,sep=' ')
      temp=temp.split()
      print len(temp)
      for i in temp:
         s=samples_count%2
         if(s==0):
            samples_tx_sensor.append(int(i,16))
         elif(s==1):
            samples_rx_sensor.append(int(i,16))
         if samples_count<=5000:
            samples_count=samples_count+1
   return samples_tx_sensor,samples_rx_sensor




def get_tof(sam_TX,sam_RX):
   mean=np.mean(sam_RX)
   normalised_tx=sam_TX-mean
   normalised_rx=sam_RX-mean
   correlate_np=np.correlate(normalised_tx,normalised_rx,'same')
   tof=np.argmax(correlate_np)-(len(normalised_tx)/2)
   return tof

def get_temp(zmq_sample_subscriber_temp):
   temp=zmq_sample_subscriber_temp
   temp=temp[1:]
   temp=hx.dump(temp1,size=4,sep=' ')
   temp=temp.split()
   counter=0
   temperature=[]
   for i in temp:
      if (counter < 100) and (counter%2==0):
         temperature.append(int(i,16))
      else:
         break
   temp_mean=np.mean(temperature)
   return temp_mean # to implement working for sampling analog values from sensor like lm-35


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
   sound_speed=20.05*math.sqrt( temp )
   speed=pulse_speed-sound_speed
   if (speed > 0):
      direction="from tx to rx"
   else:
      direction="from rx to tx"
   return speed,direction

def run():
   while True:
      temp = get_temp(zmq_sample_subscriber_temp.recv())
      samples_y_tx_sensor,samples_y_rx_sensor =get_samples(zmq_sample_subscriber_y.recv())
      tof_y = get_tof(samples_y_tx_sensor,samples_y_rx_sensor)
      samples_x_tx_sensor,samples_x_rx_sensor =get_samples(zmq_sample_subscriber_x.recv())
      tof_x=get_tof(samples_x_tx_sensor,samples_x_rx_sensor)
      windspeed_x,direction_x=find_windspeed(temp,tof_x)
      windspeed_y,direction_y=find_windspeed(temp,tof_y)
      print( "Temperature: " + str(temp) )
      print( "ToF_x: " + str(tof_x)+" tof_y: "+str(tof_y) )
      print( "Wind speed=> x: " + str(windspeed_x)+" direction_x: "+direction_x+" y: "+str(windspeed_y)+" direction_y: "+direction_y)
      time.sleep(2)



if __name__ == "__main__":
   run()