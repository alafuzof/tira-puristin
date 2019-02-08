#pragma once

#include <iostream>
#include <iomanip>
#include "bit_reader.h"
#include "bit_writer.h"

/// \brief Basic binary tree with arbitrary associated data
template <class T>
class BinaryTree {
public:
  T value; ///< Data member
  BinaryTree<T> *left_child;  ///< Left subtree or nullptr
  BinaryTree<T> *right_child; ///< Right subtree or nullptr

  /// \brief Default constructor. Initializes leaf node with data type default as value
  BinaryTree() {
    this->value = T();
    left_child = nullptr;
    right_child = nullptr;
  }

  /// \brief Constructor. Initializes leaf node with given data.
  /// \param value Value to be stored in the node
  BinaryTree(T value) {
    this->value = value;
    left_child = nullptr;
    right_child = nullptr;
  };

  /// \brief Constructor. Initializes tree with given data and children
  /// \param value Value to be stored in the node
  /// \param left Left subtree of node
  /// \param right Right subtree of node
  BinaryTree(T value, BinaryTree<T> *left, BinaryTree<T> *right) {
    this->value = value;
    left_child = left;
    right_child = right;
  };

  /// \brief Destructor. Frees memory related to tree and all its children.
  //~BinaryTree() {
  //  if(left_child != nullptr)
  //    delete left_child;
  //  if(right_child != nullptr)
  //    delete right_child;
  //};

  /// \brief Checks whether this node is a leaf
  /// \return Boolean indicating this tree has no children
  bool leaf() {
    return ((left_child == nullptr) && (right_child == nullptr));
  };

  /// \brief Convenience function for printing the node and all its children
  /// \param indent Number of indentation characters for printing
  void print_tree(unsigned int indent=0) {
    std::cout << std::setfill('-') << std::setw(indent) << "" << "Value: " << value << std::endl;
    if(left_child != nullptr) {
      std::cout << std::setfill('-') << std::setw(indent) << "" << "Left:" << std::endl;
      left_child->print_tree(indent + 4);
    } else {
      std::cout << std::setfill('-') << std::setw(indent) << "" << "Left: (none)" << std::endl;
    }
    if(right_child != nullptr) {
      std::cout << std::setfill('-') << std::setw(indent) << "" << "Right:" << std::endl;
      right_child->print_tree(indent + 4);
    } else {
      std::cout << std::setfill('-') << std::setw(indent) << "" << "Right: (none)" << std::endl;
    }
  };

  void write(BitWriter &writer) {
    int n_bytes = sizeof(value);
    unsigned char *value_bytes = (unsigned char *)(&value);
    for(int i=0; i<n_bytes; i++) {
      writer.write_byte(value_bytes[i]);
    }

    if(left_child == nullptr) {
      writer.write_bit(false);
    } else {
      writer.write_bit(true);
      left_child->write(writer);
    }

    if(right_child == nullptr) {
      writer.write_bit(false);
    } else {
      writer.write_bit(true);
      right_child->write(writer);
    }
  };

  void read(BitReader &reader) {
    int n_bytes = sizeof(value);
    unsigned char *bytes = new unsigned char[n_bytes];
    for(int i=0; i<n_bytes; i++) {
      bytes[i] = reader.read_byte();
    }
    value = *(T*)(bytes);
    delete[] bytes;

    if(reader.read_bit()) {
      left_child = new BinaryTree<T>();
      left_child->read(reader);
    } else {
      left_child = nullptr;
    }

    if(reader.read_bit()) {
      right_child = new BinaryTree<T>();
      right_child->read(reader);
    } else {
      right_child = nullptr;
    }
  };
};
