#include <sstream>
#include "gtest/gtest.h"
#include "huffman_code.h"

TEST(TestBuildTree, ZeroFrequenciesResultsInNoTree) {
  unsigned int frequencies[256] = {0};
  EXPECT_EQ(nullptr, build_tree(frequencies));
}

TEST(TestBuildTree, OneNonNullFrequencyResultsInALeaf) {
  unsigned int frequencies[256] = {0};
  frequencies[128] = 1;
  BinaryTree<unsigned char> *tree = build_tree(frequencies);
  EXPECT_NE(nullptr, tree);
  EXPECT_EQ(nullptr, tree->left_child);
  EXPECT_EQ(nullptr, tree->right_child);
  EXPECT_EQ(128, tree->value);
}

TEST(TestBuildTree, MultipleNonNullFrequenciesResultInATree) {
  unsigned int frequencies[256] = {0};
  frequencies[128] = 1;
  frequencies[129] = 1;
  BinaryTree<unsigned char> *tree = build_tree(frequencies);
  EXPECT_NE(nullptr, tree);
  EXPECT_NE(nullptr, tree->left_child);
  EXPECT_NE(nullptr, tree->right_child);
}

TEST(TestBuildCodebook, LeafResultsInSingleEntryCodebook) {
  BinaryTree<unsigned char> root('a');
  std::string *codebook = build_codebook(root);
  for(unsigned int i=0; i<256; i++) {
    if(i == 'a')
      EXPECT_STREQ("0", codebook[i].c_str());
    else
      EXPECT_STREQ("", codebook[i].c_str());
  }
}

TEST(TestBuildCodebook, TwoLeafTreeResultsInTwoEntryCodebook) {
  BinaryTree<unsigned char> root('\0');
  BinaryTree<unsigned char> left('a');
  BinaryTree<unsigned char> right('z');
  root.left_child = &left;
  root.right_child = &right;
  std::string *codebook = build_codebook(root);
  for(unsigned int i=0; i<256; i++) {
    switch(i) {
      case 'a': EXPECT_STREQ("0", codebook[i].c_str());
                break;
      case 'z': EXPECT_STREQ("1", codebook[i].c_str());
                break;
      default: EXPECT_STREQ("", codebook[i].c_str());
    }
  }
}
