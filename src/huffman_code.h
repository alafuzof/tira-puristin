#pragma once

#include <iostream>
#include <string>
#include "abstract_code.h"
#include "binary_tree.h"

/// \brief This class encodes/decodes arbitrary data using Huffman coding
class HuffmanCode : public AbstractCode{
public:
  // Destructor
  ~HuffmanCode();

  /// \brief Generates the Huffman tree and codebook based on input
  /// \param input Input stream
  /// \return 0 for success, -1 for failure
  int build_code(std::istream &input);

  /// \brief Encodes input to output using Huffman encoding
  /// \param input Input stream
  /// \param output Output stream
  /// \param verbose Enable verbose printing (default off)
  /// \param cout Output stream for verbose messages (default std::cout)
  /// \param cerr Output stream for error messages (default std::cerr)
  /// \return 0 for success, -1 for failure
  int encode(std::istream &input, std::ostream &output, bool verbose=false,
             std::ostream &cout=std::cout, std::ostream &cerr=std::cerr);
  /// \brief Decodes Huffman coded data in input to output
  /// \param input Input stream
  /// \param output Output stream
  /// \param verbose Enable verbose printing (default off)
  /// \param cout Output stream for verbose messages (default std::cout)
  /// \param cerr Output stream for error messages (default  std::cerr)
  /// \return 0 for success, -1 for failure
  int decode(std::istream &input, std::ostream &output, bool verbose=false,
             std::ostream &cout=std::cout, std::ostream &cerr=std::cerr);

  /// \brief Attempts to read the Huffman coding file header from input
  /// \param input Input stream
  /// \param cerr Output stream for error messages (default std::cerr)
  /// \return 0 for success, -1 for failure
  int read_header(std::istream &input, std::ostream &cerr=std::cerr);

  /// \brief Gets the Huffman tree for the most recent coding operation
  /// \return Huffman tree
  BinaryTree<unsigned char> *get_tree();
  /// \brief Gets the Huffman codebook for the most recent coding operation
  /// \return Huffman codebook (string array)
  std::string *get_codebook();

private:
  /// \brief Attempts to write the Huffman coding file header to output
  /// \param output Output stream
  /// \param cerr Output stream for error messages (default std::cerr)
  /// \return 0 for success, -1 for failure
  int write_header(std::ostream &output, std::ostream &cerr=std::cerr);

  BinaryTree<unsigned char> *tree; ///< Huffman tree for most recent operation
  std::string *codebook; ///< Huffman codebook for most recent operation
  //unsigned int raw_length; ///< Unencoded symbol count for most recent operation
};

/// \brief Builds Huffman tree based on the give symbol frequencies
/// \param frequencies Array of symbol 256 frequencies
/// \return Root node of the Huffman tree
BinaryTree<unsigned char> *build_tree(unsigned int *frequencies);

/// \brief Builds Huffman codebook based on the given Huffman tree
/// \param root Root node of the Huffman tree
/// \return Huffman codebook (array of strings)
std::string *build_codebook(BinaryTree<unsigned char> root);

/// \brief Prints the given Huffman codebook to output stream
/// \param codebook Huffman codebook (array of strings)
/// \param output Output stream
void print_codebook(std::string *codebook, std::ostream &output=std::cout);
