#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

void generateRandomNumbersFile(const string &fileName, size_t fileSizeBytes) {
  ofstream file(fileName, ios::binary);
  size_t bytesWritten = 0;
  random_device rd;
  mt19937 generator(rd());
  uniform_int_distribution<uint64_t> distribution(0, 999999999999);

  while (bytesWritten < fileSizeBytes) {
    uint64_t randomNumber = distribution(generator);
    string byteString = to_string(randomNumber);
    size_t bytesToWrite = min(byteString.size(), fileSizeBytes - bytesWritten);
    file.write(byteString.c_str(), bytesToWrite);
    bytesWritten += bytesToWrite;
  }
}

int main() {
  size_t targetSizeBytes = 256 * 1024 * 1024;

  string fileName = "file0";
  size_t fileSize = targetSizeBytes;
  generateRandomNumbersFile(fileName, fileSize);

  return 0;
}
