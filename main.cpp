//////////////////////////////////////////////////////////
// LoRa Image Transmission
//
// Description : Transmit a PPM Image by a LoRa modem
//
// Author : Lionel Isoz
// Version 3 - 2 août 2019
//////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <RH_RF95.h>

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

class Packet {
private:
  uint8_t packet[PACKET_INDEX_SIZE];

public:
  Packet(std::ifstream& file) {
    static uint8_t packetNumber(0);
    packet[NUMBER] = ++packetNumber;
    packet[SENT_NBR] = 0;
    packet[RECEIVED] = 0;

    int data;
    for (int i(FIRST_IMG_INDEX); i <= LAST_IMG_INDEX && !file.eof(); ++i) {
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

};


class Image {
private:
  int nbL;  // Lines nbr Image
  int nbC;  // Colums nbr Image
  int maxIntensity;
  std::vector<Packet*> packetCollection;
//  RH_RF95 rf95;
public:
  Image(const std::string& fileName) : nbL(0), nbC(0), maxIntensity(0) {
    std::ifstream file(fileName, std::ios::in);
    if (file) {
        std::string format;
        file >> format;
        if (format != "P3") std::cout << "Error, image isn't in PPM format" << std::endl;
        file >> nbL;
        file >> nbC;
        file >> maxIntensity;
        if (maxIntensity != 255) std::cout << "Error, max isn't 255" << std::endl;

        while (!file.eof()) {
          packetCollection.push_back(new Packet(file));
        }

        for (auto& packet : packetCollection) {
          packet->fill(PACKET_TOTAL_NBR, packetCollection.size());
          packet->fill(COLUMNS_NBR, nbC);
          packet->fill(LINES_NBR, nbL);
        }

        file.close();
    } else {
        std::cout << "Impossible to open the file !" << std::endl;
    }
  }

  void send() {
    for(auto& packet : packetCollection) {
      //rf95.send(packet->get(), sizeof(packet->get()));
      //rf95.waitPacketSent();
    }
  }

  void printPacketCollection() const {
    for (auto& packet : packetCollection) packet->print();
  }


  ~Image() {
    for (auto& packet : packetCollection) delete packet;
    packetCollection.clear();
  }
};


int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Error - syntax is : ./Exec imagetoSend.ppm" << std::endl;
    exit(0);
  }
  std::string fileName(argv[IMAGE_NAME]);

  Image image(fileName);
  image.send();
  image.printPacketCollection();

  return EXIT_SUCCESS;
}
