/***************************************************************************************
 * tof_analysis.c 
 *
 * Description: Converts raw ADC reads from stdin to a tof 
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 **************************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/poll.h>

#include <zmq.h>

#include "share-symlink/common_constants.h"

// TODO: These are out of date, need to take from prudaq_capture
#define TIME_BETWEEN_READS_NS    CC_ADC_BETWEEN_SAMPLES_NS
#define DELAY_TIME_NS            CC_ADC_CAPTURE_DELAY_NS
#define READS_PER_TX             CC_READS_PER_ROUND
#define BYTES_PER_READ           2

// Reads are interleaved between two channels
#define MAX_BUFFER_SIZE          (BYTES_PER_READ * READS_PER_TX * 2)

// TODO: Take as argument
#define ZMQ_HOST                "tcp://*:5555" 

double find_tof( uint16_t reads[] )
{
   size_t max = 0;
   size_t i;
   // Skip over every other value, belongs to the other channel
   for( i = 0; i < READS_PER_TX * 2; i += 2 )
   {
      if( reads[i] > reads[max] )
      {
         max = i;
      }
   }

   return DELAY_TIME_NS + max * TIME_BETWEEN_READS_NS;
}

int main(void)
{
   void* context = zmq_ctx_new();
   void* publisher = zmq_socket( context, ZMQ_PUB );
   zmq_bind( publisher, ZMQ_HOST ); 

   printf( "ADC Driver starting\n" );
   uint8_t buffer[ MAX_BUFFER_SIZE ];
   uint16_t* reads = (uint16_t*)buffer;   // Assumes little-endian

   while(1)
   {
      if( fread( buffer, sizeof buffer, 1, stdin ) != 0 )
      {
            // reads and buffer are aliased
            double tof = find_tof( reads );
            printf( "Time of flight: %f ns\n", tof );

            char zmsg[80];
            snprintf( zmsg, sizeof zmsg, "%f", tof );
            zmq_send( publisher, zmsg, strlen(zmsg), 0 );
      }
   }

   zmq_close( publisher );
   zmq_ctx_destroy( context );

   return 0;
}
