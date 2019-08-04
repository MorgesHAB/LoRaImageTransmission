/*
Original code by : https://github.com/Snootlab/lora_chisterapi
Edited by : Lionel Isoz
*/
#include <RH_RF95.h>
#include <iostream>
using namespace std;

int main() {
    RH_RF95 rf95;

    wiringPiSetupGpio();

    if (!rf95.init()) {
        fprintf(stderr, "Init failed\n");
        exit(0);
    }

    rf95.setFrequency(868.0); /* Mhz */

    while(1) {
        if( rf95.available() )
        {
            uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t len = sizeof(buf);

            if (rf95.recv(buf, &len))
            {
                cout << +buf[0] << endl;
            }
        }
        usleep(1);
    }
    return EXIT_SUCCESS;
}