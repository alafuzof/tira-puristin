#include <cmath>
#include <iomanip>
#include <sstream>
#include "file_analyzer.h"
#include "priority_queue.h"
#include "huffman_code.h"

// Currently the code equates symbol with 8-bit char
// TODO: wide char support?
#define NUM_SYMBOLS 256

FileAnalyzer::FileAnalyzer() {
  this->m_symbol_count = new unsigned int[NUM_SYMBOLS]();
  this->m_length = 0;
}

FileAnalyzer::~FileAnalyzer() {
  delete[] this->m_symbol_count;
}

void FileAnalyzer::analyze(std::istream &input) {
  // First reset the collected stats
  for(int i=0; i<NUM_SYMBOLS; i++) {
    this->m_symbol_count[i] = 0;
  }
  this->m_length = 0;

  // Save current stream position
  std::streampos pos = input.tellg();

  // Iterate through the input and count character occurrence
  char c;
  while(input.get(c)) {
    this->m_symbol_count[(unsigned char)c]++;
    this->m_length++;
  }

  // Restore stream position
  input.clear(); // Clear to reset the fail bit from the final get() above
  input.seekg(pos);

  // Build the Huffman code and get the codebook
  BinaryTree<unsigned char> *huffman_tree = build_tree(m_symbol_count);
  codebook = build_codebook(*huffman_tree);
  //delete huffman_tree;
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

  // Sort the symbols by their occurrence/probability
  float *prob = probabilities();
  PriorityQueue<char> pq(NUM_SYMBOLS, MAX_PRIORITY);
  for(int i=0; i<NUM_SYMBOLS; i++) {
    if(prob[i] > 0.0)
      pq.push((char)i, prob[i]);
  }
  delete[] prob;

  // Print the most common symbols and their frequencies and Huffman codes
  if(!pq.empty()) {
    std::cout << "Top " << std::min((int)pq.length(), 20) << " symbols:" << std::endl;
    std::cout << "Ord\tVal\tChar\tCount\tCode" << std::endl;
    for(int i=0; i<std::min((int)pq.length(), 20); i++) {
      Element<char> el = pq.pop();
      if(el.value < 33 || el.value > 126)
        std::cout << std::setw(2) << i+1 << ".\t"
                  << std::setw(3) << (int)el.value << "\t \t"
                  << this->m_symbol_count[(int)el.value] << "\t"
                  << codebook[(int)el.value] << std::endl;
      else
        std::cout << std::setw(2) << i+1 << ".\t"
                  << std::setw(3) << (int)el.value << "\t "
                  << el.value << "\t"
                  << this->m_symbol_count[(int)el.value] << "\t"
                  << codebook[(int)el.value] << std::endl;
    }
  }
}

unsigned int FileAnalyzer::length() {
  return this->m_length;
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
    prob[i] = (float)m_symbol_count[i] / m_length;
  }
  return prob;
}

unsigned int *FileAnalyzer::frequencies() {
  unsigned int *freq = new unsigned int[NUM_SYMBOLS];
  for(int i=0; i<NUM_SYMBOLS; i++) {
    freq[i] = m_symbol_count[i];
  }
  return freq;
}
