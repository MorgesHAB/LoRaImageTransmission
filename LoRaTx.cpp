/*
Original code by : https://github.com/Snootlab/lora_chisterapi
Edited by : Lionel Isoz
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <RH_RF95.h>

#define NODE_NUMBER 8

RH_RF95 rf95;

uint8_t msg[200];

/* Compose a message to be sent in a MorgesHabPacket : MHPacket */
/*Syntax is:
  byte 0: node number
  byte 1: msg type
  byte 2: msg length (max 255)
  byte 3 & ... : ASCII encoded value, null terminated
*/
void writeMHPacket(uint8_t type, char *m) {
  msg[0] = NODE_NUMBER;
  msg[1] = type;
  msg[2] = (uint8_t)strlen(m);
  strcpy((char *)&msg[3], m); // We should use strncpy !!!!
}


void setup() {
     wiringPiSetupGpio();

     if (!rf95.init()) {
         fprintf(stderr, "Init failed\n");
         exit(0);
     }
     /* Tx power is from +5 to +23 dBm */
     rf95.setTxPower(23);
     /* There are different configurations you can
        find in lib/radiohead/RH_RF95.h at line 437 */
     rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
     rf95.setFrequency(868.0); /* Mhz */
}

int main(int argc, char **argv) {

   // signal(SIGINT, sigint_handler);
  //  signal(SIGALRM, sigalarm_handler);

    //alarm(3);

    setup();

    //writeMHPacket(i-1, argv[i]);
    uint8_t data[] = "MorgesHAB";
    rf95.send(data, sizeof(data));

    //rf95.send(msg, sizeof(msg));
    rf95.waitPacketSent();
    //alarm(3);

    return EXIT_SUCCESS;
}
