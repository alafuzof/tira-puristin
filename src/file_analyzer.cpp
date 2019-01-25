#include <cmath>
#include <sstream>
#include "file_analyzer.h"

FileAnalyzer::FileAnalyzer() {
  // Currently the code equates symbol with 8-bit char
  // TODO: wide char support?
  this->m_symbol_count = new unsigned int[256]();
  this->m_length = 0;
}

FileAnalyzer::~FileAnalyzer() {
  delete[] this->m_symbol_count;
}

void FileAnalyzer::analyze(std::istream &input) {
  // First reset the collected stats
  for(int i=0; i<256; i++) {
    this->m_symbol_count[i] = 0;
  }
  this->m_length = 0;

  // Iterate through the input and count character occurrence
  char c;
  while(input.get(c)) {
    this->m_symbol_count[(unsigned char)c]++;
    this->m_length++;
  }
}

void FileAnalyzer::analyze(const std::string &input) {
  // Use string stream to mediate between input and primary analyze method
  std::istringstream stream_input(input);
  this->analyze(stream_input);
}

void FileAnalyzer::print_report() {
  std::cout << "File length (bytes):\n"
            << this->m_length << '\n' << std::endl;

  float ent = this->entropy();
  std::cout << "First order entropy (bits / symbol):\n"
            << ent << '\n' << std::endl;

  // The minimum file size is assuming an entropy based single symbol encoding
  // scheme. The actual minimum file size may be lower, if there is additional
  // structure to the data
  std::cout << "\"Minimum\" file size:\n"
            << ent*this->m_length << " bits\n"
            << ent*this->m_length/8 << " bytes\n" << std::endl;

  std::cout << "Byte counts:" << std::endl;
  for(int i=0; i<256; i++) {
    if(m_symbol_count[i] != 0) { // Only show occurrence for symbols seen
      std::cout << i << " - " << this->m_symbol_count[i] << std::endl;
    }
  }
}

unsigned int FileAnalyzer::length() {
  return this->m_length;
}

float FileAnalyzer::entropy() {

  float res = 0.0;
  for(int i=0; i<256; i++) {
    float p = (float)this->m_symbol_count[i] / this->m_length;
    if(p > 0) // By convention 0*log(0) = 0
      res -= p * std::log2(p);
  }
  return res;
}
