#include <cmath>
#include "file_analyzer.h"

FileAnalyzer::FileAnalyzer() {
  this->m_byte_counts = new unsigned int[256]();
  this->m_length = 0;
}

FileAnalyzer::~FileAnalyzer() {
  delete[] this->m_byte_counts;
}

void FileAnalyzer::analyze(std::istream &input) {
  for(int i=0; i<256; i++) {
    this->m_byte_counts[i] = 0;
  }
  this->m_length = 0;

  char c;
  while(input.get(c)) {
    this->m_byte_counts[(unsigned char)c]++;
    this->m_length++;
  }
}

void FileAnalyzer::print_report() {
  std::cout << "File length (bytes):\n"
            << this->m_length << '\n' << std::endl;

  float ent = this->entropy();
  std::cout << "First order entropy (bits / character):\n"
            << ent << '\n' << std::endl;

  std::cout << "Minimum file size:\n"
            << ent*this->m_length << " bits\n"
            << ent*this->m_length/8 << " bytes\n" << std::endl;

  std::cout << "Byte counts:" << std::endl;
  for(int i=0; i<256; i++) {
    std::cout << i << " - " << this->m_byte_counts[i] << std::endl;
  }
}

unsigned int FileAnalyzer::length() {
  return this->m_length;
}

float FileAnalyzer::entropy() {
  float res = 0.0;
  for(int i=0; i<256; i++) {
    float p = (float)this->m_byte_counts[i] / this->m_length;
    if(p > 0)
      res -= p * std::log2(p);
  }
  return res;
}
