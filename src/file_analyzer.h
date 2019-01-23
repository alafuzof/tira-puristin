#pragma once
#include <iostream>

class FileAnalyzer {
public:
  FileAnalyzer();
  ~FileAnalyzer();

  void analyze(std::istream &input);

private:
  int *byte_counts;
};
