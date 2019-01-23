#include <iostream>
#include <fstream>
#include "file_analyzer.h"

int main(int argc, char *argv[]) {
  if(argc < 3) {
    std::cout << "Usage" << std::endl;
    std::cout << std::endl;
    std::cout << '\t' << "puristin [operation] <path-to-file>" << std::endl;
  }

  if(argc == 3 && (std::string(argv[1]) == "a" || std::string(argv[1]) == "analyze")) {
    std::ifstream input_file(argv[2], std::ifstream::binary);
    FileAnalyzer fa;
    fa.analyze(input_file);
    fa.print_report();
  }

  //std::cout << "Hello World!" << std::endl;
  return 0;
}
