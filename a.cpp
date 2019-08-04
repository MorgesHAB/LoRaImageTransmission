#include <iostream>
#include <fstream>

using namespace std;

int main() {
  std::ifstream file("test.txt", std::ios::in);

  if (file) {
    uint8_t a;
    file >> a;
    cout << +a << endl;
    file.close();
  }

  return 0;
}
