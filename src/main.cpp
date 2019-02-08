#include <iostream>
#include <fstream>
#include "file_analyzer.h"
#include "huffman_code.h"

int main(int argc, char *argv[]) {
  if(argc == 3 && (std::string(argv[1]) == "a" || std::string(argv[1]) == "analyze")) {
    std::ifstream input_file(argv[2], std::ifstream::binary);
    FileAnalyzer fa;
    fa.analyze(input_file);
    fa.print_report();
    return 0;
  }

  if(argc == 4 && (std::string(argv[1]) == "c" || std::string(argv[1]) == "compress")) {
    std::ifstream input_file(argv[2], std::ifstream::binary);
    std::ofstream output_file(argv[3], std::ofstream::binary);
    HuffmanCode hc;
    hc.encode(input_file, output_file, true);
    return 0;
  }

  if(argc == 4 && (std::string(argv[1]) == "d" || std::string(argv[1]) == "decompress")) {
    std::ifstream input_file(argv[2], std::ifstream::binary);
    std::ofstream output_file(argv[3], std::ofstream::binary);
    HuffmanCode hc;
    hc.decode(input_file, output_file, true);
    return 0;
  }

  std::cout << "Usage" << std::endl;
  std::cout << std::endl;
  std::cout << '\t' << "puristin [operation] <path-to-file> (path-to-save)" << std::endl;

  //std::cout << "Hello World!" << std::endl;
  return 0;
}
