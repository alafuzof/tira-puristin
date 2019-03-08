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

  /// \brief Recursively writes a compact binary representation for the tree
  /// \param writer BitWriter object used to output the binary representation
  void write(BitWriter &writer) {
    // Write the value of the current root node
    int n_bytes = sizeof(value);
    unsigned char *value_bytes = (unsigned char *)(&value);
    for(int i=0; i<n_bytes; i++) {
      writer.write_byte(value_bytes[i]);
    }

    // If there is no left child, write a '0'-bit
    if(left_child == nullptr) {
      writer.write_bit(false);
    } else { // Otherwise write '1'-bit and then recursively the entire left subtree
      writer.write_bit(true);
      left_child->write(writer);
    }

    // If there is no right child, write a '0'-bit
    if(right_child == nullptr) {
      writer.write_bit(false);
    } else { // Otherwise write '1'-bit and then recursively the entire right subtree
      writer.write_bit(true);
      right_child->write(writer);
    }
  };

  void read(BitReader &reader) {
    // Read the value of the current node
    int n_bytes = sizeof(value);
    unsigned char *bytes = new unsigned char[n_bytes];
    for(int i=0; i<n_bytes; i++) {
      bytes[i] = reader.read_byte();
    }
    value = *(T*)(bytes);
    delete[] bytes;

    // If we see a '1' bit, there is a left child, so we read it recursively
    if(reader.read_bit()) {
      left_child = new BinaryTree<T>();
      left_child->read(reader);
    } else { // Otherwise, no left child
      left_child = nullptr;
    }

    // If we see a '1' bit, there is a right child, so we read it recursively
    if(reader.read_bit()) {
      right_child = new BinaryTree<T>();
      right_child->read(reader);
    } else { // Otherwise, no left child
      right_child = nullptr;
    }
  };
};

/// \brief Check whether two trees are equal (structure and values)
/// \param a Tree A
/// \param b Tree B
/// \return True if A == B, false if A != B
template <class T>
bool tree_equality(BinaryTree<T> &a, BinaryTree<T> &b) {
  bool flag = true;
  flag = flag && (a.value == b.value);
  if((a.left_child != nullptr && b.left_child == nullptr) ||
     (a.left_child == nullptr && b.left_child != nullptr))
     return false;
  if(a.left_child != nullptr)
    flag = flag && tree_equality(*a.left_child, *b.left_child);
  if(a.right_child != nullptr)
    flag = flag && tree_equality(*a.right_child, *b.right_child);
  return flag;
}
