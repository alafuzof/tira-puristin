#pragma once

#include <iostream>

enum Operation {ANALYZE, COMPRESS, DECOMPRESS, UNKNOWN};

class CLI {
public:
  CLI(int argc, char *argv[]);

  int run();

  Operation parse_operation();

  int analyze(std::ostream &output=std::cout);
  int compress(std::ostream &output=std::cout);
  int compress_huffman(std::ostream &output=std::cout);
  int compress_lzw(std::ostream &output=std::cout);
  int decompress(std::ostream &output=std::cout);
  int print_help(std::ostream &output=std::cout);

private:
  int argc;
  char **argv;
};
