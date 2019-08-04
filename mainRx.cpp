/*
Original code by : https://github.com/Snootlab/lora_chisterapi
Edited by : Lionel Isoz
*/
#include <RH_RF95.h>
#include <iostream>
#define PACKET_INDEX_SIZE   250
#define PACKET_MAX_SIZE     250 // bytes  // real max = 255
#define IMAGE_NAME          1   // argv

// Packet Index - encapsulation order - use : packet[NAME]
#define NUMBER              0   //
#define SENT_NBR            1   //   change for each packet
#define RECEIVED            2   //
#define LENGTH              3   //
#define PACKET_TOTAL_NBR    4   ///
#define COLUMNS_NBR         5   ///  constant for an image
#define LINES_NBR           6   ///
#define FIRST_IMG_INDEX     7   //// IMAGE part
#define LAST_IMG_INDEX      246 ////  => 80 pixel 3x80 = 240

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
            uint8_t packet[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t len = sizeof(packet);

            if (rf95.recv(packet, &len))
            {
              std::cout << "--------------------- PACKET PRINT --------------------------" << std::endl;
              std::cout << " | Packet n° " << +packet[NUMBER] << " / " << +packet[PACKET_TOTAL_NBR]
                        << " | Length : " << +packet[LENGTH]
                        << " | Sent " << +packet[SENT_NBR] << " time(s)"
                        << " | Received : " << ((+packet[RECEIVED])?"YES!":"NO!")
                        << " | Image -  " << +packet[LINES_NBR] << " Lines & "
                                         << +packet[COLUMNS_NBR] << " Columns";
                std::cout << " | Image Data : " << std::endl;
                for (int i(FIRST_IMG_INDEX); i <= LAST_IMG_INDEX; ++i) {
                  std::cout << +packet[i] << " ";
                  if (i % 50 == 0) std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
        usleep(1);
    }
    return EXIT_SUCCESS;
}
