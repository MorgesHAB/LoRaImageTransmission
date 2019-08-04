#include <iostream>

using namespace std;

int main() {
  uint16_t a(300);

  uint8_t b(a); // 204   à droite
  uint8_t c(a >> 8); //16 à gauche
  uint16_t x(c << 8 | b);
  cout << 3*+a << " " << +b << " " << +c << " " << +x << endl;
  uint s(0);

  return 0;
}
