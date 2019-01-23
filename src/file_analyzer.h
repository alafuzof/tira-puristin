#pragma once
#include <iostream>

class FileAnalyzer {
public:
  FileAnalyzer();
  ~FileAnalyzer();

  void analyze(std::istream &input);

  void print_report();

  unsigned int length();
  float entropy();

private:
  unsigned int *m_byte_counts;
  unsigned int m_length;
};
