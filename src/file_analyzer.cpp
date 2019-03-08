#include <cmath>
#include <iomanip>
#include <sstream>
#include "file_analyzer.h"

// The code equates symbol with 8-bit char
#define NUM_SYMBOLS 256

FileAnalyzer::FileAnalyzer() {
  symbol_count = new unsigned int[NUM_SYMBOLS]();
  input_length = 0;
}

FileAnalyzer::~FileAnalyzer() {
  delete[] symbol_count;
}

void FileAnalyzer::analyze(std::istream &input) {
  // First reset the collected stats
  for(int i=0; i<NUM_SYMBOLS; i++) {
    symbol_count[i] = 0;
  }
  input_length = 0;

  // Save current stream position
  std::streampos pos = input.tellg();

  // Iterate through the input and count character occurrence
  char c;
  while(input.get(c)) {
    symbol_count[(unsigned char)c]++;
    input_length++;
  }

  // Restore stream position
  input.clear(); // Clear to reset the fail bit from the final get() above
  input.seekg(pos);
}

void FileAnalyzer::analyze(const std::string &input) {
  // Use string stream to mediate between input and primary analyze method
  std::istringstream stream_input(input);
  this->analyze(stream_input);
}

unsigned int FileAnalyzer::length() {
  return input_length;
}

float FileAnalyzer::entropy() {
  float *prob = probabilities();
  float res = 0.0;
  for(int i=0; i<NUM_SYMBOLS; i++) {
    if(prob[i] > 0) // By convention 0*log(0) = 0
      res -= prob[i] * std::log2(prob[i]);
  }
  delete[] prob;
  return res;
}

float *FileAnalyzer::probabilities() {
  float *prob = new float[NUM_SYMBOLS];
  for(int i=0; i<NUM_SYMBOLS; i++) {
    prob[i] = (float)symbol_count[i] / input_length;
  }
  return prob;
}

unsigned int *FileAnalyzer::frequencies() {
  return symbol_count;
}
