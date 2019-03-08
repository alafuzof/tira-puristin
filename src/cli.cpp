#include <iostream>
#include <fstream>
#include "file_analyzer.h"
#include "huffman_code.h"
#include "lzw_code.h"
#include "priority_queue.h"
#include "cli.h"


CLI::CLI(int argc, char *argv[]): argc(argc), argv(argv) {}

int CLI::run() {
  if(argc >= 2) {
    Operation op = parse_operation();
    switch(op) {
      case ANALYZE: if(analyze() == -1) {
                      print_help();
                      return -1;
                    } else
                      return 0;
      case COMPRESS: if(compress() == -1) {
                       print_help();
                       return -1;
                     } else
                       return 0;
      case DECOMPRESS: if(decompress() == -1) {
                         print_help();
                         return -1;
                       } else
                         return 0;
      default: print_help();
               return -1;
    }
  }

  print_help();
  return -1;
}

CLI::Operation CLI::parse_operation() {
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
    output << "Please specify compression type: huffman or lzw!" << std::endl;
    return -1;
  }

  std::string compression = argv[2];
  if(compression == "huffman" || compression == "h")
    return compress_huffman();
  if(compression == "lzw")
    return compress_lzw();

  output << "Unknown compression type: " << compression << std::endl;
  return -1;
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
  if(!input_file.good()) {
    output << "Could not open input file " << argv[3] << " for reading!" << std::endl;
    return -1;
  }
  std::ofstream output_file(argv[3], std::ofstream::binary);
  if(!output_file.good()) {
    output << "Could not open output file " << argv[4] << " for reading!" << std::endl;
    return -1;
  }

  HuffmanCode hc;
  LZWCode lzw;
  if(hc.read_header(input_file) == 0) {
    input_file.seekg(0, input_file.beg);
    int status = hc.decode(input_file, output_file);
    if(status == 0) {
      output << "Decoded " << hc.get_unencoded_symbol_count() << " symbols to "
             << hc.get_encoded_symbol_count() << " symbols" << std::endl;
      output << "Decompressed " << hc.get_unencoded_length() << " bytes to "
             << hc.get_encoded_length() << " bytes" << std::endl;
      output << "Compression ratio: " << hc.get_compression_ratio() << std::endl;
      output << "Elapsed time: " << hc.get_total_time() << " s ("
             << hc.get_time_per_symbol()*1000*1000*1000 << " ns per symbol)" << std::endl;
    }
    return status;
  }

  input_file.seekg(0, input_file.beg);
  if(lzw.read_header(input_file) == 0) {
    input_file.seekg(0, input_file.beg);
    lzw.decode(input_file, output_file);
    return 0;
  } else {
    output << "Input file does not seem to be packed by puristin!" << std::endl;
    return -1;
  }
}

int CLI::compress_huffman(std::ostream &output) {
  if(argc < 4) {
    output << "Please specify an input file for compression!" << std::endl;
    return -1;
  }
  if(argc < 5) {
    output << "Please specify an output file for compression!" << std::endl;
    return -1;
  }

  std::ifstream input_file(argv[3], std::ifstream::binary);
  if(!input_file.good()) {
    output << "Could not open input file " << argv[3] << " for reading!" << std::endl;
    return -1;
  }
  std::ofstream output_file(argv[4], std::ofstream::binary);
  if(!output_file.good()) {
    output << "Could not open output file " << argv[4] << " for reading!" << std::endl;
    return -1;
  }
  HuffmanCode hc;
  int status = hc.encode(input_file, output_file, false);
  if(status == 0) {
    output << "Encoded " << hc.get_unencoded_symbol_count() << " symbols to "
           << hc.get_encoded_symbol_count() << " symbols" << std::endl;
    output << "Compressed " << hc.get_unencoded_length() << " bytes to "
           << hc.get_encoded_length() << " bytes" << std::endl;
    output << "Compression ratio: " << hc.get_compression_ratio() << std::endl;
    output << "Elapsed time: " << hc.get_total_time() << " s ("
           << hc.get_time_per_symbol()*1000*1000*1000 << " ns per symbol)" << std::endl;
  }

  return status;
}

int CLI::compress_lzw(std::ostream &output) {
  if(argc < 4) {
    output << "Please specify a bitrate (9-20 bits/symbol) for compression!" << std::endl;
    return -1;
  }
  if(argc < 5) {
    output << "Please specify an input file for compression!" << std::endl;
    return -1;
  }
  if(argc < 6) {
    output << "Please specify an output file for compression!" << std::endl;
    return -1;
  }

  int n_bits = 0;
  try {
    n_bits = std::stoi(argv[3]);
    if(n_bits < 9 || n_bits > 20)
      throw;
  } catch(...) {
    output <<  "Could not parse " << argv[3] << " as bitrate between 9 and 20 bits!" << std::endl;
    return -1;
  }

  std::ifstream input_file(argv[4], std::ifstream::binary);
  if(!input_file.good()) {
    output << "Could not open input file " << argv[4] << " for reading!" << std::endl;
    return -1;
  }
  std::ofstream output_file(argv[5], std::ofstream::binary);
  if(!output_file.good()) {
    output << "Could not open output file " << argv[5] << " for reading!" << std::endl;
    return -1;
  }

  LZWCode lzw;
  lzw.encode(input_file, output_file, n_bits);
  return 0;
}

void CLI::print_help(std::ostream &output) {
  output << "Usage\n" << std::endl;
  output << "    puristin [operation] (options) <path-to-file> <path-to-save>" << std::endl;

  output << "\nExamples:\n" << std::endl;

  output << "  Analyze file:" << std::endl;
  output << "    puristin analyze file" << std::endl;
  output << "    puristin a file\n" << std::endl;

  output << "  Compress file:" << std::endl;
  output << "    puristin compress huffman file file.compressed" << std::endl;
  output << "    puristin c huffman file file.compressed" << std::endl;
  output << "    puristin compress lzw 10 file file.compressed" << std::endl;
  output << "    puristin c lzw 16 file file.compressed\n" << std::endl;

  output << "  Decompress file:" << std::endl;
  output << "    puristin decompress file.compressed file" << std::endl;
  output << "    puristin d file.compressed file" << std::endl;
}
