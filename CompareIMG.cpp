#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
  std::string fileName1(argv[1]), fileName2(argv[2]);
  std::ifstream file1(fileName1, std::ios::in);
  std::ifstream file2(fileName2, std::ios::in);

  if (file1 && file2) {
    while (!file1.eof() || !file2.eof()) {
      static count(0);
      int data1, data2;
      file1 >> data1;
      file2 >> data2;
      ++count;
      if (data1 != data2)
        cout << "diff at " << count  << " : " data1 << " vs " << data2 << endl;
    }

    file1.close();
    file2.close();
  }
  return 0;
}
