#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
  std::string fileName1(argv[1]);
  std::ifstream file1(fileName1, std::ios::in);
  if (file1) {
    cout << "file open" << endl;
    int data1, data2;
    file1 >> data1;
    cout << data1 << endl;
    file1.close();
  }
  return 0;
}
