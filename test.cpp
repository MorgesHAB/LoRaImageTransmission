#include <iostream>

using namespace std;

int main() {
  int b(1000);
  uint8_t a(12);
  uint8_t c[] = "MHPacket";
  a = b;

  cout << (int) a << " " << c << " " << sizeof(c) << endl;
  cout << sizeof(a) << endl;

  uint8_t x[10];
  x[0] = 2;
  x[1] = 3;

  cout << x << "   -   " << &x[0] << endl;
  cout << unsigned(x[0]) << "   -   " <<unsigned(x[1])<< endl;

  uint8_t y[200];
  y[0] = 12;

  uint8_t q(123);

  cout << +q << endl;


  return 0;
}
