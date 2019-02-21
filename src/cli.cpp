#include <iostream>
#include <fstream>
#include "file_analyzer.h"
#include "huffman_code.h"
#include "cli.h"


CLI::CLI(int argc, char *argv[]): argc(argc), argv(argv) {}

int CLI::run() {
  if(argc >= 2) {
    Operation op = parse_operation();
    switch(op) {
      case ANALYZE: return analyze();
      case COMPRESS: return compress();
      case DECOMPRESS: return decompress();
      default: return print_help();
    }
  } else
    return print_help();
}

Operation CLI::parse_operation() {
  std::string op = argv[1];
  if(op == "a" || op == "analyze")
    return ANALYZE;
  if(op == "c" || op == "compress")
    return COMPRESS;
  if(op == "d" || op == "decompress")
    return DECOMPRESS;
  return UNKNOWN;
}

int CLI::analyze(std::ostream &output) {
  if(argc < 3) {
    output << "Please specify a file to analyze!" << std::endl;
    return -1;
  }

  std::ifstream input_file(argv[2], std::ifstream::binary);
  FileAnalyzer fa;
  fa.analyze(input_file);
  fa.print_report();
  return 0;
}

int CLI::compress(std::ostream &output) {
  if(argc < 3) {
    output << "Please specify input and output files for compression!" << std::endl;
    return -1;
  }
  if(argc < 4) {
    output << "Please specify an output file for compression!" << std::endl;
    return -1;
  }

  std::ifstream input_file(argv[2], std::ifstream::binary);
  std::ofstream output_file(argv[3], std::ofstream::binary);
  HuffmanCode hc;
  hc.encode(input_file, output_file, true);
  return 0;
}

int CLI::decompress(std::ostream &output) {
  if(argc < 3) {
    output << "Please specify input and output files for decompression!" << std::endl;
    return -1;
  }
  if(argc < 4) {
    output << "Please specify an output file for decompression!" << std::endl;
    return -1;
  }

  std::ifstream input_file(argv[2], std::ifstream::binary);
  std::ofstream output_file(argv[3], std::ofstream::binary);
  HuffmanCode hc;
  hc.decode(input_file, output_file, true);
  return 0;
}

int CLI::print_help(std::ostream &output) {
  output << "Usage\n" << std::endl;
  output << '\t' << "puristin [operation] <path-to-file> (path-to-save)" << std::endl;
  return 0;
}
