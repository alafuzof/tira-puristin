#pragma once

#include <iostream>
#include <string>
#include "binary_tree.h"

class HuffmanCode {
public:
  // Generates the Huffman tree and codebook based on input
  // \param input Input stream
  // \return 0 for success, -1 for failure
  int build_code(std::istream &input);

  // Encodes input to output using Huffman encoding
  // \param input Input stream
  // \param output Output stream
  // \param verbose Enable verbose printing
  // \return 0 for success, -1 for failure
  int encode(std::istream &input, std::ostream &output, bool verbose=false);
  // Decodes Huffman coded data in input to output
  // \param input Input stream
  // \param output Output stream
  // \param verbose Enable verbose printing
  // \return 0 for success, -1 for failure
  int decode(std::istream &input, std::ostream &output, bool verbose=false);

  // Attempts to read the Huffman coding file header from input
  // \param input Input stream
  // \return 0 for success, -1 for failure
  int read_header(std::istream &input);

  // Gets the Huffman tree for the most recent coding operation
  // \return Huffman tree
  BinaryTree<unsigned char> *get_tree();
  // Gets the Huffman codebook for the most recent coding operation
  // \return Huffman codebook (string array)
  std::string *get_codebook();

private:
  // Attempts to write the Huffman coding file header to output
  // \param output Output stream
  // \return 0 for success, -1 for failure
  int write_header(std::ostream &output);

  BinaryTree<unsigned char> *tree; ///< Huffman tree for most recent operation
  std::string *codebook; ///< Huffman codebook for most recent operation
  unsigned int raw_length; ///< Unencoded symbol count for most recent operation
};

BinaryTree<unsigned char> *build_tree(unsigned int *frequencies);
std::string *build_codebook(BinaryTree<unsigned char> root);
void print_codebook(std::string *codebook, std::ostream &output=std::cout);
