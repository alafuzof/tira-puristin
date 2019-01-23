#include "file_analyzer.h"

FileAnalyzer::FileAnalyzer() {
  this->byte_counts = new int[256]();
}

FileAnalyzer::~FileAnalyzer() {
  delete[] this->byte_counts;
}

void FileAnalyzer::analyze(std::istream &input) {
  for(int i=0; i<256; i++) {
    this->byte_counts[i] = 0;
  }

  char c;
  while(input.get(c)) {
    this->byte_counts[(unsigned char)c]++;
  }

  std::cout << "Byte counts:" << std::endl;
  for(int i=0; i<256; i++) {
    std::cout << i << " - " << this->byte_counts[i] << std::endl;
  }
}
