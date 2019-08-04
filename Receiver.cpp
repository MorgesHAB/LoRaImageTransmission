//////////////////////////////////////////////////////////
// LoRa Image Transmission
//
// Description : Transmit a PPM Image by a LoRa modem
//
// Author : Lionel Isoz
// Version 3 - 2 août 2019
//
//   ---Reception modem---
//////////////////////////////////////////////////////////
#include <iostream>
#include <RH_RF95.h>
#include "define.h"

RH_RF95 rf95;

void setupLoRaPHY() {
  wiringPiSetupGpio();

  if (!rf95.init()) {
    std::cout << "Init failed" << std::endl;
     exit(0);
  }
  rf95.setFrequency(FREQUENCY); /* MHz */
  rf95.setTxPower(TX_POWER);
  switch(LORA_MODE) {
    case MEDIUM_RANGE :
      rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128);
      break;
    case FAST_SHORT_RANGE :
      rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
      break;
    case SLOW_LONG_RANGE :
      rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
      break;
    case MAX_RANGE :
      rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
      break;
    }
}

void print(uint8_t& packet) {
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


int main() {
  while(true) {
    if (rf95.available()) {
      uint8_t packet[PACKET_INDEX_SIZE];
      uint8_t len = sizeof(packet);

      if (rf95.recv(packet, &len)) {
        print(packet);
      }
    }
    usleep(RECEPTION_SLEEP_TIME);
  }
  return EXIT_SUCCESS;
}
