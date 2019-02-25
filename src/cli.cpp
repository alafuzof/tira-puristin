#include <iostream>
#include <fstream>
#include "file_analyzer.h"
#include "huffman_code.h"
#include "priority_queue.h"
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
  //fa.print_report();

  // Build the Huffman code and get the codebook
  BinaryTree<unsigned char> *huffman_tree = build_tree(fa.frequencies());
  std::string *codebook = nullptr;
  if(huffman_tree != nullptr)
    codebook = build_codebook(*huffman_tree);

  output << "File length (bytes):\n"
         << fa.length() << '\n' << std::endl;

  output << "First order entropy (bits / symbol):\n"
         << fa.entropy() << '\n' << std::endl;

  // The minimum file size is assuming an entropy based single symbol encoding
  // scheme. The actual minimum file size may be lower, if there is additional
  // structure to the data
  output << "\"Minimum\" file size:\n"
           << fa.entropy()*fa.length() << " bits\n"
           << (fa.entropy()*fa.length())/8 << " bytes\n" << std::endl;

  // Sort the symbols by their occurrence/probability
  float *prob = fa.probabilities();
  PriorityQueue<char> pq(256, MAX_PRIORITY); // FIXME: 256 hardcoding
  for(int i=0; i<256; i++) { // FIXME: 256 hardcoding
   if(prob[i] > 0.0)
     pq.push((char)i, prob[i]);
  }
  delete[] prob;

  unsigned int *freqs = fa.frequencies();

  // Print the most common symbols and their frequencies and Huffman codes
  if(!pq.empty()) {
    output << "Top " << std::min((int)pq.length(), 20) << " symbols:" << std::endl;
    output << "Ord\tVal\tChar\tCount\tCode" << std::endl;
    for(int i=0; i<std::min((int)pq.length(), 20); i++) {
      Element<char> el = pq.pop();
      if(el.value < 33 || el.value > 126)
        output << std::setw(2) << i+1 << ".\t"
               << std::setw(3) << (int)el.value << "\t \t"
               << freqs[(int)el.value] << "\t"
               << codebook[(int)el.value] << std::endl;
      else
        output << std::setw(2) << i+1 << ".\t"
               << std::setw(3) << (int)el.value << "\t "
               << el.value << "\t"
               << freqs[(int)el.value] << "\t"
               << codebook[(int)el.value] << std::endl;
    }
  }

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
