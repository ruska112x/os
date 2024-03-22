#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
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
  size_t numFiles = 20;
  size_t targetSizeBytes = 256 * 1024 * 1024;
  vector<thread> threads;
  for (size_t i = 0; i < numFiles; ++i) {
    string fileName = "file" + to_string(i);
    size_t fileSize = targetSizeBytes;
    threads.emplace_back(generateRandomNumbersFile, fileName, fileSize);
  }

  for (auto &thread : threads) {
    thread.join();
  }

  size_t totalFilesSize = 0;
  for (size_t i = 0; i < numFiles; ++i) {
    string fileName = "file" + to_string(i);
    ifstream file(fileName, ios::binary | ios::ate);
    totalFilesSize += file.tellg();
  }

  cout << "Total size of generated files: "
       << totalFilesSize / (1024.0 * 1024 * 1024) << " GB" << endl;

  return 0;
}
