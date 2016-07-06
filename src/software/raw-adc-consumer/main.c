/***************************************************************************************
 * MAIN.C
 *
 * Description: Converts raw ADC reads given via character device to time of flight
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 **************************************************************************************/

#include <stdint.h>
#include <stdio.h>
//#include <unistd.h>
//#include <string.h>
#include <fcntl.h>
#include <sys/poll.h>

// TODO: Get these value from a shared source along with the firmware
#define TIME_BETWEEN_READS_NS    166.7
#define DELAY_TIME_NS            0
#define READS_PER_TX             2000
#define BYTES_PER_READ           2

#define CHARACTER_DEVICE_PATH    "/dev/rpmsg_pru31"
#define MAX_BUFFER_SIZE          (BYTES_PER_READ * READS_PER_TX)


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

int main(void)
{
   uint8_t buffer[ MAX_BUFFER_SIZE ];
   uint16_t* reads = (uint16_t*)buffer;   // Assumes little-endian

   struct pollfd pollfds[1];

   pollfds[0].fd = open( CHARACTER_DEVICE_PATH, O_RDWR );

   if( pollfds[0].fd < 0 )
   {
      printf( "Unable to open char device." );
      return -1;
   }

   // Firmware needs an initial write to grab metadata
      // msg contents irrelevant
   // TODO: Should probably handle errors better
   while( write( pollfds[0].fd, "s", 1 ) < 0 )
   {
      printf( "Problem with initial send. Retrying..." );
   }


   while(1)
   {
      // Grab a whole run and then process
      // TODO: Figure out of this is sufficient or if incremental processing is required for performance
      size_t total_bytes = 0;
      while( total_bytes < MAX_BUFFER_SIZE )
      {
         total_bytes += read( pollfds[0].fd, buffer + total_bytes, MAX_BUFFER_SIZE - total_bytes );
      }

      // reads and buffer are aliased
      double tof = find_tof( reads );
      printf( "Time of flight: %d ns", tof );
   }

	return 0;
}
