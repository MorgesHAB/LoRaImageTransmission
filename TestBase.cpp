/*
Original code by : https://github.com/Snootlab/lora_chisterapi
Edited by : Lionel Isoz
*/
#include <RH_RF95.h>

int main() {
    RH_RF95 rf95;

    wiringPiSetupGpio();

    if (!rf95.init()) {
       fprintf(stderr, "Init failed\n");
       exit(0);
    }

    rf95.setFrequency(868.0); /* Mhz */

    uint8_t data[] = "MorgesHAB";
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();

    return EXIT_SUCCESS;
}