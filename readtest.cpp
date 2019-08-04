#include <iostream>
#include <fstream>

using namespace std;

int main() {
  std::ifstream file("img.ppm", std::ios::in);
  if (file) {
      std::string format;
      int nbC, nbL, maxIntensity;
      file >> format;
      if (format != "P3") std::cout << "Error, image isn't in PPM format" << std::endl;
      file >> nbL;
      file >> nbC;
      file >> maxIntensity;
      if (maxIntensity != 255) std::cout << "Error, max isn't 255" << std::endl;

      int data;
      while (!file.eof()) {
        file >> data;
        std::cout << "--> " << data << " eof : " << file.eof() << std::endl;
      }

      file.close();
  } else {
      std::cout << "Impossible to open the file !" << std::endl;
  }


  return 0;
}
