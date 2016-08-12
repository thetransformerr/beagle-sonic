/***************************************************************************************
 * MAIN.C
 *
 * Description: Converts raw ADC reads given via character device to time of flight
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

#include "common_constants.h"

// TODO: Get these value from a shared source along with the firmware
#define TIME_BETWEEN_READS_NS    166.7
#define DELAY_TIME_NS            0
#define READS_PER_TX             CC_READS_PER_ROUND
#define BYTES_PER_READ           2

#define CHARACTER_DEVICE_PATH    "/dev/rpmsg_pru31"
#define MAX_BUFFER_SIZE          (BYTES_PER_READ * READS_PER_TX)

#define CMD_BUFFER_SIZE          80

// TODO: Take as argument
#define ZMQ_HOST                "tcp://*:5555" 

// TODO: Figure out how to share these values with the firmware...
//       Firmware just cares about bits in R30/R31. Need some way to relate the two nicely
char* pins_array[] = { "P8_45", "P8_46", "P8_43", "P8_44", "P8_41", "P8_42", "P8_39", "P8_40", "P8_27", "P8_29", "P8_28", "P8_30" };

double find_tof( uint16_t reads[] )
{
   size_t max = 0;
   size_t i;
   for( i = 0; i < READS_PER_TX; i++ )
   {
      if( reads[i] > reads[max] )
      {
         max = i;
      }
   }

   return DELAY_TIME_NS + max * TIME_BETWEEN_READS_NS;
}

void set_pins( char setting[] )
{
   char cmd[ CMD_BUFFER_SIZE ];

   size_t i;
   for( i = 0; i < (sizeof pins_array / sizeof pins_array[0]); i++ )
   {
      // TODO: Check for and handle errors somehow
      char* pin = pins_array[i];
      snprintf( cmd, sizeof cmd, "config-pin %s %s", pin, setting );
      printf( "Running: %s\n", cmd );
      // TODO: Check for and handle errors somehow
      system( cmd );
   }
}

void set_pins_out()
{
   set_pins( "pruout" );
}

void set_pins_in()
{
   set_pins( "pruin" );
}

int main(void)
{
   void* context = zmq_ctx_new();
   void* publisher = zmq_socket( context, ZMQ_PUB );
   zmq_bind( publisher, ZMQ_HOST ); 

   printf( "ADC Driver starting\n" );
   uint8_t buffer[ MAX_BUFFER_SIZE ];
   uint16_t* reads = (uint16_t*)buffer;   // Assumes little-endian

   uint8_t msg;

   struct pollfd pollfds[1];

   pollfds[0].fd = open( CHARACTER_DEVICE_PATH, O_RDWR );

   if( pollfds[0].fd < 0 )
   {
      printf( "Unable to open char device.\n" );
      return -1;
   }

   // Firmware needs an initial write to grab metadata
      // msg contents irrelevant
   // TODO: Should probably handle errors better
   while( write( pollfds[0].fd, "s", 1 ) < 0 )
   {
      printf( "Problem with initial send. Retrying...\n" );
   }


   while(1)
   {
      read( pollfds[0].fd, &msg, sizeof msg );

      switch( msg )
      {
         case CC_REQ_CONFIG:
            printf( "Setting pins to gpio out\n" );
            set_pins_out();
            msg = CC_FIN_CONFIG;
            while( write( pollfds[0].fd, &msg, 1 ) < 0 )
            {
               printf( "Problem with fin_config send. Retrying...\n" );
            }
            break;

         case CC_REQ_INPUT_READY:
            printf( "Setting pins to gpio in\n" );
            set_pins_in();
            msg = CC_FIN_INPUT_READY;
            while( write( pollfds[0].fd, &msg, 1 ) < 0 )
            {
               printf( "Problem with fin_config send. Retrying...\n" );
            }
            break;

         case CC_REQ_CONSUMER:
            msg = CC_FIN_CONSUMER;
            while( write( pollfds[0].fd, &msg, 1 ) < 0 )
            {
               printf( "Problem with fin_consumer send. Retrying...\n" );
            }

            printf( "Gathering data...\n" );
            // Grab a whole run and then process
            // TODO: Figure out of this is sufficient or if incremental processing is required for performance
            size_t total_bytes = 0;
            while( total_bytes < MAX_BUFFER_SIZE )
            {
               printf( "   %u of %u bytes\n", total_bytes, MAX_BUFFER_SIZE);
               total_bytes += read( pollfds[0].fd, buffer + total_bytes, MAX_BUFFER_SIZE - total_bytes );
            }

            // reads and buffer are aliased
            double tof = find_tof( reads );
            printf( "Time of flight: %f ns\n", tof );

            char zmsg[80];
            snprintf( zmsg, sizeof zmsg, "%f", tof );
            zmq_send( publisher, zmsg, strlen(zmsg), 0 );
            break;
      }
   }

   close( pollfds[0].fd );
   zmq_close( publisher );
   zmq_ctx_destroy( context );

   return 0;
}
