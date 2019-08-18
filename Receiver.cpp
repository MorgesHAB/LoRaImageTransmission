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
#include <stdlib.h> // to use system()
#include <RH_RF95.h>
#include "define.h"

void setupLoRaPHY(RH_RF95& rf95) {
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
            << " | Image -  " << +linesNbr << " Lines & " << +colNbr << " Columns";
  std::cout << " | Image Data : " << std::endl;
  for (int i(FIRST_DATA_INDEX); i <= LAST_DATA_INDEX; ++i) {
    std::cout << +packet[i] << " ";
    if (i % 50 == 0) std::cout << std::endl;
  }
  std::cout << std::endl;
}

void buildImage(std::vector<uint8_t*> &packetCollection) {
  std::ofstream file(NAME_RX_FILE, std::ios::out);
  if (file) {
    for (auto& packet : packetCollection) {
      if (packet != nullptr) {
        if ((packet[NUMBER_L] << 8 | packet[NUMBER_R]) == 1) { // if first packet
          file << "P3" << std::endl;
          file << +(packet[COLUMNS_NBR_L] << 8 | packet[COLUMNS_NBR_R])
               << " " << +(packet[LINES_NBR_L] << 8 | packet[LINES_NBR_R]) << std::endl;
          file << 255 << std::endl; // Max value
        }
        // print content of the image
        for (int i(FIRST_DATA_INDEX); i <= packet[LENGTH]; ++i) {
          if ((i-FIRST_DATA_INDEX) % 30 == 0) file << std::endl;
          file << +packet[i] << " ";
        }
      } else { // print black
         for (int i(0); i < BYTE_PER_PACKET; ++i) {
          if ((i-FIRST_DATA_INDEX) % 30 == 0) file << std::endl;
          file << BLACK << " ";
        }
      }
    }
    file.close();
  } else {
      std::cout << "Impossible to open the file !" << std::endl;
  }
  for (auto& packet : packetCollection) delete[] packet;    // release memory
  packetCollection.clear();
  system("convert ImageRx.ppm ImageRx.jpg");
  std::cout << "Building finished : the image is available as ImageRx.ppm " <<
                "& ImageRx.jpg" << std::endl;
  exit(0);
}

void askForMissingPacket(std::vector<uint8_t*> &packetCollection, RH_RF95& rf95) {
  uint8_t packet[PACKET_INDEX_SIZE];
  uint8_t index(FIRST_DATA_INDEX);
  for (uint16_t nbr(0); nbr < packetCollection.size() && index < LAST_DATA_INDEX; ++nbr) { // warning indexes
    if (packetCollection[nbr] == nullptr) { // not received
      packet[index] = (nbr+1) >> 8;
      packet[index+1] = nbr+1;
      packet[LENGTH] = index+1;
      packet[NBR_PACKET_TO_SEND_AGAIN] = (index + 2 - FIRST_DATA_INDEX) / 2;
      index+=2;
    }
  }
  rf95.send(packet, PACKET_INDEX_SIZE);
  rf95.waitPacketSent();
  std::cout << "Missing Packets request sent" << std::endl;
}

void sendReceptionConfirmation(RH_RF95& rf95) {
  uint8_t packet[PACKET_INDEX_SIZE];
  packet[NBR_PACKET_TO_SEND_AGAIN] = 0;
  rf95.send(packet, PACKET_INDEX_SIZE);
  rf95.waitPacketSent();
  std::cout << "Reception confirmation sent" << std::endl;
}

void TCP(uint8_t* packet, RH_RF95& rf95) {
  uint16_t linesNbr(packet[LINES_NBR_L] << 8 | packet[LINES_NBR_R]);
  uint16_t colNbr(packet[COLUMNS_NBR_L] << 8 | packet[COLUMNS_NBR_R]);
  int bytePerPacket(BYTE_PER_PACKET);
  int totalPacket(3 * +linesNbr * +colNbr / bytePerPacket + 1);
  uint16_t packetNbr(packet[NUMBER_L] << 8 | packet[NUMBER_R]);

  static std::vector<uint8_t*> packetCollection(totalPacket, nullptr);
  static int packetcounter(0);
  if (packetCollection[packetNbr-1] == nullptr) {
    packetCollection[packetNbr-1] = packet;
    ++packetcounter;
  }

  if (packetNbr == totalPacket /*|| packetcounter == totalPacket*/) {
    bool allReceived(true);
    for (int nbr(0); nbr < totalPacket; ++nbr) { // warning indexes
      if (packetCollection[nbr] == nullptr) {
        std::cout << "Packet : " << nbr+1 << " not received" << std::endl;
        allReceived = false;
      }
    }
    std::cout << "packet all received : " << ((allReceived)?"YES":"NO") << std::endl;
    if (allReceived) {
      sendReceptionConfirmation(rf95);
      buildImage(packetCollection);
    }
    else {
      std::cout << "Some packets are missing, would you like to build the " <<
                    "image whatever [1], wait that they come [2] or start the automatic TCP manager [3]" << std::endl
                    << "Type 1, 2 or 3 to continue : ";
      std::string mode;
      do { std::cin >> mode; } while(mode != "1" && mode != "2" && mode != "3");
      std::cout << "ok mode " << mode << " is activated" << std::endl;
      if (mode == "1") buildImage(packetCollection);
      if (mode == "3") askForMissingPacket(packetCollection, rf95);
    }
  }
}


int main() {
  RH_RF95 rf95;
  setupLoRaPHY(rf95);
  std::cout << "\nRecpetion enabled... Waiting for packets\n" << std::endl;
  while(true) {
    if (rf95.available()) {
      uint8_t* packet = new uint8_t[PACKET_INDEX_SIZE];
      uint8_t len = PACKET_INDEX_SIZE;

      if (rf95.recv(packet, &len)) {
        print(packet);
        TCP(packet, rf95);
      }
    }
    usleep(RECEPTION_SLEEP_TIME);
  }
  return EXIT_SUCCESS;
}
