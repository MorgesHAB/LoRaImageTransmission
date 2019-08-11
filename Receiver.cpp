//////////////////////////////////////////////////////////
// LoRa Image Transmission
//
// Description : Transmit a PPM Image by a LoRa modem
//
// Author : Lionel Isoz
// Version 11 août 2019
//
//   ---Reception modem---
//////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <vector>
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

void print(uint8_t* packet) {
  uint16_t linesNbr(packet[LINES_NBR_L] << 8 | packet[LINES_NBR_R]);
  uint16_t colNbr(packet[COLUMNS_NBR_L] << 8 | packet[COLUMNS_NBR_R]);
  int bytePerPacket(BYTE_PER_PACKET);
  int totalPacket(3 * +linesNbr * +colNbr / bytePerPacket + 1);
  uint16_t packetNbr(packet[NUMBER_L] << 8 | packet[NUMBER_R]);
  std::cout << "--------------------- PACKET PRINT --------------------------" << std::endl;
  std::cout << " | Packet n° " << packetNbr << " / " << totalPacket
            << " | Length : " << +packet[LENGTH]
            << " | Sent " << +packet[SENT_NBR] << " time(s)"
            << " | Received : " << ((+packet[RECEIVED])?"YES!":"NO!")
            << " | Image -  " << +linesNbr << " Lines & " << +colNbr << " Columns";
  std::cout << " | Image Data : " << std::endl;
  for (int i(FIRST_IMG_INDEX); i <= LAST_IMG_INDEX; ++i) {
    std::cout << +packet[i] << " ";
    if (i % 50 == 0) std::cout << std::endl;
  }
  std::cout << std::endl;
}

void buildImage(std::vector<uint8_t*> &packetCollection) {
  std::ofstream file(NAME_RX_FILE, std::ios::out);
  if (file) {
    for (auto& packet : packetCollection) {
      std::cout << "packet " << (packet != nullptr) << std::endl;
      if (packet != nullptr) {
        if ((packet[NUMBER_L] << 8 | packet[NUMBER_R]) == 1) { // if first packet
          file << "P3" << std::endl;
          file << +(packet[COLUMNS_NBR_L] << 8 | packet[COLUMNS_NBR_R])
               << " " << +(packet[LINES_NBR_L] << 8 | packet[LINES_NBR_R]) << std::endl;
          file << 255 << std::endl; // Max value
        }
        // print content of the image
        for (int i(FIRST_IMG_INDEX); i <= +packet[LENGTH]; ++i) {
          if ((i-FIRST_IMG_INDEX) % 30 == 0) file << std::endl;
          file << +packet[i] << " ";
        }
      } else {
         // print black
      }
    }
    file.close();
  } else {
      std::cout << "Impossible to open the file !" << std::endl;
  }
}

void TCP(uint8_t* packet) {
  uint16_t linesNbr(packet[LINES_NBR_L] << 8 | packet[LINES_NBR_R]);
  uint16_t colNbr(packet[COLUMNS_NBR_L] << 8 | packet[COLUMNS_NBR_R]);
  int bytePerPacket(BYTE_PER_PACKET);
  int totalPacket(3 * +linesNbr * +colNbr / bytePerPacket + 1);
  uint16_t packetNbr(packet[NUMBER_L] << 8 | packet[NUMBER_R]);

  static std::vector<uint8_t*> packetCollection(totalPacket);
  if (packetCollection[packetNbr-1] == nullptr) {
    packetCollection[packetNbr-1] = packet;
    std::cout << "save" << std::endl;
  }

  packet[RECEIVED] = true;
  static std::vector<bool> packetsCheck(totalPacket, false);
  packetsCheck[packetNbr] = true;
  if (packetNbr == totalPacket) {
    for (int nbr(1); nbr < totalPacket; ++nbr) {
      if (packetsCheck[nbr] == false)
        std::cout << "Packet : " << nbr << " not received" << std::endl;
    }
    //buildImage(packetCollection);
    for (auto& e : packetCollection) std::cout << +e[NUMBER_R] << std::endl;
  }
}


int main() {
  setupLoRaPHY();
  while(true) {
    if (rf95.available()) {
      uint8_t packet[PACKET_INDEX_SIZE];
      uint8_t len = sizeof(packet);

      if (rf95.recv(packet, &len)) {
        print(packet);
        TCP(packet);
      }
    }
    usleep(RECEPTION_SLEEP_TIME);
  }
  return EXIT_SUCCESS;
}
