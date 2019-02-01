#pragma once

#include <iostream>
#include <string>
#include "binary_tree.h"

class HuffmanCode {
public:
  void build_code(std::istream &input);

  void encode(std::istream &input, std::ostream &output);
  void decode(std::istream &input, std::ostream &output);

  BinaryTree<unsigned char> *get_tree();
  std::string *get_codebook();

private:
  BinaryTree<unsigned char> *tree;
  std::string *codebook;
};

BinaryTree<unsigned char> *build_tree(unsigned int *frequencies);
std::string *build_codebook(BinaryTree<unsigned char> root);
