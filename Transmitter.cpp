//////////////////////////////////////////////////////////
// LoRa Image Transmission
//
// Description : Transmit a PPM Image by a LoRa modem
//
// Author : Lionel Isoz
// Version 11 août 2019
//
//   ---Transmitter modem---
//////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h> // to use system()
#include <RH_RF95.h>
#include "define.h"

class Packet {
private:
  uint8_t packet[PACKET_INDEX_SIZE];

public:
  Packet(std::ifstream& file) {
    static uint16_t packetNumber(0);
    packet[NUMBER_L] = (++packetNumber) >> 8;
    packet[NUMBER_R] = packetNumber;
    packet[SENT_NBR] = 0;

    int data;
    for (int i(FIRST_DATA_INDEX); i <= LAST_DATA_INDEX && !file.eof(); ++i) {
      file >> data;
      uint8_t data8(data);
      packet[i] = data8;
      packet[LENGTH] = i;
    }
  }

  uint8_t* get() {
    return packet;
  }

  void fill(uint8_t index, uint8_t data) {
    packet[index] = data;
  }

  void print() const {
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

};


class Image {
private:
  uint16_t nbL;  // Lines nbr Image
  uint16_t nbC;  // Colums nbr Image
  unsigned int maxIntensity;
  std::vector<Packet*> packetCollection;
  RH_RF95 rf95;
public:
  Image(const std::string& fileName) : nbL(0), nbC(0), maxIntensity(0) {
    setupLoRaPHY();
    std::ifstream file(fileName, std::ios::in);
    if (file) {
        std::string format;
        file >> format;
        if (format != "P3") std::cout << "Error, image isn't in PPM format" << std::endl;
        file >> nbC;
        file >> nbL;
        file >> maxIntensity;
        if (maxIntensity != 255) std::cout << "Error, max isn't 255" << std::endl;

        while (!file.eof()) {
          packetCollection.push_back(new Packet(file));
        }

        for (auto& packet : packetCollection) {
          packet->fill(COLUMNS_NBR_L, nbC >> 8);
          packet->fill(COLUMNS_NBR_R, nbC);
          packet->fill(LINES_NBR_L, nbL >> 8);
          packet->fill(LINES_NBR_R, nbL);
        }

        file.close();
    } else {
        std::cout << "Impossible to open the file !" << std::endl;
    }
  }

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

  void sendPacketNbr(size_t nbr) {
    if (nbr > 0 && nbr <= packetCollection.size()) {
      ++packetCollection[nbr-1]->get()[SENT_NBR];
      rf95.send(packetCollection[nbr-1]->get(), PACKET_INDEX_SIZE);
      rf95.waitPacketSent();
      std::cout << "Packet " << +(packetCollection[nbr-1]->get()[NUMBER_L]
                     << 8 | packetCollection[nbr-1]->get()[NUMBER_R])
                << "/" << packetCollection.size() << " sent" << std::endl;
      usleep(TRANSMISSION_SLEEP_TIME);
    } else {
      std::cout << "Packet number invalid" << std::endl;
    }
  }

  void send() {
    for(size_t i(1); i <= packetCollection.size(); ++i) sendPacketNbr(i);
  }

  void printPacketCollection() const {
    for (auto& packet : packetCollection) packet->print();
  }

  void sendMissingPacket() {
    while(true) {
      if (rf95.available()) {
        uint8_t* packet = new uint8_t[PACKET_INDEX_SIZE];
        uint8_t len = PACKET_INDEX_SIZE;

        if (rf95.recv(packet, &len)) {
          if (packet[NBR_PACKET_TO_SEND_AGAIN] == 0) {
            std::cout << "All packets have been received, process finished" << std::endl;
            exit(0);
          }
          for (int i(FIRST_DATA_INDEX); i <= LAST_DATA_INDEX; i+=2) {
            uint16_t nbr(packet[i] << 8 | packet[i+1]);
            sendPacketNbr(nbr);
          }
        }
      }
      usleep(RECEPTION_SLEEP_TIME);
    }
  }

  ~Image() {
    for (auto& packet : packetCollection) delete packet;
    packetCollection.clear();
  }
};

void takePicture(std::string& fileName) {
  // Take a picture with the raspicam
  system("raspistill -o ImageTx.jpg -hf -vf -w 300 -h 200");
  // install before :  sudo apt install imagemagick
  system("convert -compress none ImageTx.jpg ImageTx.ppm");
  // convert inverse :  convert img.ppm img.jpg
  std::cout << "Picture taken and converted to ppm format" << std::endl;
  fileName = "ImageTx.ppm";
}


int main(int argc, char* argv[]) {
  if (argc != 2 && argc != 3) {
    std::cout << "Error - syntax is : ./Transmitter imagetoSend.ppm" << std::endl;
    std::cout << "               or : ./Transmitter livePicture" << std::endl;
    std::cout << "               or : ./Transmitter image.ppm [packetNbr]" << std::endl;
    return EXIT_SUCCESS;
  }
  std::string fileName(argv[IMAGE_NAME]);
  if (fileName == KEY_WORD_PICTURE) takePicture(fileName);

  Image image(fileName);

  if (argc == 3) {
    size_t packetNbr = std::stoi(argv[PACKET_NUMBER]);
    image.sendPacketNbr(packetNbr);
    return EXIT_SUCCESS;
  }
  image.send();
  std::cout << "All packets have been sent 1 time" << std::endl;
  std::cout << "Waiting for response of the receiver..." << std::endl;
  while(true) {
    image.sendMissingPacket();
  }

  return EXIT_SUCCESS;
}
