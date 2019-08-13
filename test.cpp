#include <iostream>
#include "define.h"

using namespace std;

int main() {

  uint8_t packet2[PACKET_INDEX_SIZE];
  uint8_t* packet = new uint8_t[PACKET_INDEX_SIZE];
  uint8_t len = PACKET_INDEX_SIZE;
  uint8_t len2 = sizeof(packet2);


  cout << "length " << +len << " " << +len2<< endl;

  for (int i(0); i< 10 ;++i) {
    packet[i] = i;
  }

  for (int i(0); i< 10 ;++i) {
    cout << +packet[i] << endl;
  }

  delete[] packet;

  return 0;
}
