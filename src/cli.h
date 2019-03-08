#pragma once

#include <iostream>

/// \brief Simple command line interface for the (de-)compression functionality
class CLI {
public:
  /// \brief Constructor. Initializes the object with the given arguments
  /// \param argc Number of command line arguments (+1 for program name)
  /// \param argv Array of null terminated strings, one for each argument
  CLI(int argc, char *argv[]);

  /// \brief Run the program
  /// \return Final status of the program (0 for success, -1 for failure)
  int run();

private:
  /// Convenience names for the different operations
  enum Operation {ANALYZE, COMPRESS, DECOMPRESS, UNKNOWN};

  /// \brief Parse the operation argument
  /// \return Operation value of the operation argument
  Operation parse_operation();

  /// \brief Parse and run an analysis command
  /// \param output Output stream for console output to the user
  /// \return Final status of the analysis (0 for success, -1 for failure)
  int analyze(std::ostream &output=std::cout);
  /// \brief Parse and run an compression command
  /// \param output Output stream for console output to the user
  /// \return Final status of the compression (0 for success, -1 for failure)
  int compress(std::ostream &output=std::cout);
  /// \brief Parse and run a Huffman compression command
  /// \param output Output stream for console output to the user
  /// \return Final status of the compression (0 for success, -1 for failure)
  int compress_huffman(std::ostream &output=std::cout);
  /// \brief Parse and run an LZW compression command
  /// \param output Output stream for console output to the user
  /// \return Final status of the compression (0 for success, -1 for failure)
  int compress_lzw(std::ostream &output=std::cout);
  /// \brief Parse and run an decompression command
  /// \param output Output stream for console output to the user
  /// \return Final status of the decompression (0 for success, -1 for failure)
  int decompress(std::ostream &output=std::cout);

  /// \brief Print a help message
  /// \param output Output stream for console output to the user
  void print_help(std::ostream &output=std::cout);

  int argc;    ///< Number of command line arguments (+1 for program name)
  char **argv; ///< Command line arguments
};
