/*
 * comms.c
 *
 * Description: Implementation for comms.h.
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

#include "comms.h"

#include <stdint.h>

#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>
#include "resource_table_1.h"

#include "common_constants.h"
#include "pin_control.h"


// Defines mostly lifted from pru-support-package lab5
#define HOST_INT_BIT    31

#define TO_ARM_HOST     18
#define FROM_ARM_HOST   19
#define CHAN_NAME       "rpmsg-pru"
#define CHAN_DESC       "Channel 31"
#define CHAN_PORT       31

#define VIRTIO_CONFIG_S_DRIVER_OK   4
// End lifted defines

#define MAX_MSG_LENGTH (RPMSG_BUF_SIZE - 16)


static struct pru_rpmsg_transport transport;
static uint16_t src, dst, len;

static uint8_t buffer[ MAX_MSG_LENGTH ];


static void send_msg( void* msg, uint16_t msg_len )
{
   size_t i = 0;
   while( i < msg_len )
   {
      uint16_t len = (i + MAX_MSG_LENGTH > msg_len ? msg_len - i : MAX_MSG_LENGTH);

      // TODO: Probably ignores some conditions that can't be fixed by waiting
      while( pru_rpmsg_send( &transport, src, dst, msg + i, len ) != PRU_RPMSG_SUCCESS );

      i += MAX_MSG_LENGTH;
   }
}

static uint8_t recieve_msg()
{
   while(1)
   {
      if( read_pin( HOST_INT_BIT ) )
      {
         CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

         if( pru_rpmsg_receive( &transport, &dst, &src, buffer, &len ) == PRU_RPMSG_SUCCESS )
         {
            // TODO: Is there a better way to do this?
            // No way more than a byte is needed though
            return buffer[0];
         }
      }
   }
}

static wait_for_req_handled( uint8_t req, uint8_t resp )
{
   do
   {
      send_msg( &req, 1 );
   } while( recieve_msg() != resp );
}


void CO_wait_init()
{
   volatile uint8_t* status;

   // Enable OCP master port access for the PRU
   CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

   // Clear incoming message flag
   CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

   // Wait for linux driver ready
   status = &resourceTable.rpmsg_vdev.status;
   while(!(*status & VIRTIO_CONFIG_S_DRIVER_OK));

   pru_rpmsg_init( &transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST );

   // Create RPMsg channel
   while( pru_rpmsg_channel( RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS );

   // Wait for incoming message to grab src and dst and check init msg - happens implicitly in receive_msg
   // Note: can't use wait_for_req_handled here, as it sends before it receives
   // TODO: Figure out error handling
   recieve_msg();
}

void CO_wait_config()
{
   wait_for_req_handled( CC_REQ_CONFIG, CC_FIN_CONFIG );
}

void CO_transfer_reads( void* buffer )
{
   wait_for_req_handled( CC_REQ_CONSUMER, CC_FIN_CONSUMER );
   send_msg( buffer, CC_READS_PER_ROUND * 2 ); // TODO: Remove hardcoded read size
}


#undef TO_ARM_HOST
#undef CHAN_NAME
#undef CHAN_DESC
#undef CHAN_PORT
#undef VIRTIO_CONFIG_S_DRIVER_OK
#undef MAX_MSG_LENGTH
