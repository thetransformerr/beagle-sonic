/*
Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.  See the License for the specific language governing
permissions and limitations under the License.
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <libgen.h>
#include <string.h>
#include <fcntl.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <zmq.h>
#include <sys/poll.h>
#include <signal.h>
#include <time.h>
// Header for sharing info between PRUs and application processor
#include "shared_header.h"

static int bCont = 1;
#define ZMQ_HOST_x    "tcp://*:5555" 
#define ZMQ_HOST_y    "tcp://*:5556"
#define ZMQ_HOST_temp    "tcp://*:5557" 

// the PRU clock speed used for GPIO clock generation
#define PRU_CLK 200e6

void set_freq(float freq_hz){   //Set freq in pwm module
   FILE *fp;
   fp=fopen("/sys/class/pwm/pwmchip0/pwm0/period","w");
   if (fp == NULL){
     perror("GPIO: write failed to open file to set frequency ");
     return;
   }
   float period_s = 1.0f/freq_hz;
   unsigned int period_ns=(unsigned int)period_s*1000000000;
   fprintf(fp,"%u",period_ns);
   fclose(fp);
   return;
}




//------set duty cycle in nanoseconds,should not be greater than period----
void set_DutyCycle(unsigned int duty_ns){
   FILE *fp;
   fp=fopen("/sys/class/pwm/pwmchip0/pwm0/duty_cycle","w");
   if (fp == NULL){
     perror("GPIO: write failed to open file  to set duty cycle");
     return;
   }
   fprintf(fp,"%u",duty_ns);
   fclose(fp);
   return;
}




//works only when freq is 40kHz
void set_DutyCycle_40(float percentage){
   FILE *fp;
   fp=fopen("/sys/class/pwm/pwmchip0/pwm0/duty_cycle","w");
   if (fp == NULL){
     perror("GPIO: write failed to open file to set duty cycle");
     return;
   }
   if ((percentage>100.0f)||(percentage<0.0f)){
     perror("enter value between 0 to 100");
     return;
   }
   float duty=25000*(percentage/100.0f);
   unsigned int duty_ns=(unsigned int)duty;
   fprintf(fp,"%u",duty_ns);
   fclose(fp);
   return;
}




void pwm_enable(){
  FILE *fp;
   fp=fopen("/sys/class/pwm/pwmchip0/pwm0/enable","w");
   if (fp == NULL){
     perror("GPIO: write failed to open file to enable pwm ");
     return;
   }
  fprintf(fp,"%d",1);
  fclose(fp);
  return;}



void pwm_disable(){
  FILE *fp;
   fp=fopen("/sys/class/pwm/pwmchip0/pwm0/enable","w");
   if (fp == NULL){
     perror("GPIO: write failed to open file to disable pwm");
     return;
   }
  fprintf(fp,"%d",0);
  fclose(fp);
  return;}




void sig_handler (int sig) {
  // break out of reading loop
  bCont = 0;
  return;
}


int main (int argc, char **argv) {
  if (geteuid() != 0) {
    fprintf(stderr, "Must be root. Try again with sudo.\n");
    return EXIT_FAILURE;
  }

  if (argc != 3) {
    fprintf(stderr, "Usage: %s pru0_code.bin pru1_code.bin\n", argv[0]);
    return EXIT_FAILURE;
  }

  // install signal handler to catch ctrl-C
  if (SIG_ERR == signal(SIGINT, sig_handler)) {
    perror("Warn: signal handler not installed %d\n");
  }

  // If this segfaults, make sure you're executing as root.
  prussdrv_init();
  if (0 != prussdrv_open(PRU_EVTOUT_0)) {
    fprintf(stderr, "prussdrv_open() failed\n");
    return EXIT_FAILURE;
  }
  
  void* context_x = zmq_ctx_new();
  void* publisher_x = zmq_socket( context_x, ZMQ_PUB );
  
  void* context_y = zmq_ctx_new();
  void* publisher_y = zmq_socket( context_y, ZMQ_PUB );

  void* context_temp = zmq_ctx_new();
  void* publisher_temp = zmq_socket( context_temp, ZMQ_PUB );

  zmq_bind( publisher_x, ZMQ_HOST_x);
  zmq_bind( publisher_y, ZMQ_HOST_y);
  zmq_bind( publisher_temp, ZMQ_HOST_temp);  

  pwm_enable();
  set_freq(40000);//frequency supplied in hz
  set_DutyCycle_40(50.00);//in percentage,function works only for 40 kHz, use another function described above for other freq
  
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_pruintc_init(&pruss_intc_initdata);

  // Get pointer into the 8KB of shared PRU DRAM where prudaq expects
  // to share params with prus and the main cpu
  volatile pruparams_t *pparams = NULL;
  prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void**)&pparams);

  // Pointer into the DDR RAM mapped by the uio_pruss kernel module.
  volatile uint32_t *shared_ddr = NULL;
  prussdrv_map_extmem((void**)&shared_ddr);
  unsigned int shared_ddr_len = prussdrv_extmem_size();

  unsigned int physical_address = prussdrv_get_phys_addr((void*)shared_ddr);

  fprintf(stderr,
          "%uB of shared DDR available.\n Physical (PRU-side) address:%x\n",
         shared_ddr_len, physical_address);
  fprintf(stderr, "Virtual (linux-side) address: %p\n\n", shared_ddr);

  pparams->physical_addr = physical_address;
  pparams->ddr_len       = shared_ddr_len;

  prussdrv_exec_program(0, argv[1]);
  prussdrv_exec_program(1, argv[2]);

  volatile uint32_t *read_pointer = shared_ddr;
  volatile uint32_t *buffer_end = shared_ddr + (shared_ddr_len / sizeof(*shared_ddr));

  // Dummy variable so compiler doesn't optimize away our data.

  int64_t bytes_read = 0;

  while (bCont) {
    fprintf(stderr,"sleeping for 2 seconds\n");
    sleep(2);
    pwm_enable();
    usleep(250);
    pwm_disable();
    usleep(1000);
    uint32_t *write_pointer_virtual =prussdrv_get_virt_addr(pparams->shared_ptr);

    while (read_pointer != write_pointer_virtual) {
      // Copy to a local array so we're not working in special slow DMA ram
      uint16_t amplitudes[6];
      memcpy(amplitudes, (void *) read_pointer, 12);

      for (int i = 0; i < 6; i++) {
        switch(i){
          case 0:zmq_send( publisher_x, &amplitudes[i], sizeof(amplitudes[0]), 0 );  //tx for y
          case 1:zmq_send( publisher_y, &amplitudes[i], sizeof(amplitudes[0]), 0 );  //tx for x
          case 2:zmq_send(publisher_temp,&amplitudes[i],sizeof(amplitudes[0]),0);   //input for temperature
          case 3:zmq_send(publisher_x,&amplitudes[i],sizeof(amplitudes[0]),0);  //rx input channel for x  
          case 4:zmq_send(publisher_y,&amplitudes[i],sizeof(amplitudes[0]),0);  //rx for y
        }
          bytes_read += sizeof(amplitudes[0]);
      }

      // Occasionally report to stderr
      if (bytes_read % (1048576) == 0) {
        fprintf(stderr, "Processed %lldMB\n", bytes_read / 1048576);
        fprintf(stderr,
                "Most recent amplitude for channel 0:%d  1:%d 2:%d 3:%d 4:%d  5:%d \n",
                amplitudes[0], amplitudes[1], amplitudes[2], amplitudes[3], amplitudes[4], amplitudes[5]);
      }

      read_pointer += (12 / sizeof(*read_pointer));
      if (read_pointer >= buffer_end) {
        read_pointer = shared_ddr;
      }
    }
  }

  fprintf(stderr, "All done\n");
  prussdrv_pru_disable(0);
  prussdrv_pru_disable(1);
  prussdrv_exit();
  return 0;
}
