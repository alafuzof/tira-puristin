#include <sstream>
#include "gtest/gtest.h"
#include "huffman_code.h"
#include "file_analyzer.h"

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

class HuffmanCodeTest : public ::testing::Test {
public:
  HuffmanCodeTest() {
    hc = new HuffmanCode();
    iss = new std::istringstream("Lorem ipsum dolor sit amet, consectetur "
      "adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore "
      "magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
      "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute "
      "irure dolor in reprehenderit in voluptate velit esse cillum dolore eu "
      "fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, "
      "sunt in culpa qui officia deserunt mollit anim id est laborum.",
      std::ios_base::in|std::ios_base::binary);
    oss = new std::ostringstream();
    ss = new std::stringstream();
  };
  ~HuffmanCodeTest() {
    delete hc;
  };
  HuffmanCode *hc;
  std::istringstream *iss;
  std::ostringstream *oss;
  std::stringstream *ss;
};

TEST_F(HuffmanCodeTest, HuffmanTreeCanBeNavigatedUsingCodebook) {
  hc->encode(*iss, *ss, false);
  BinaryTree<unsigned char> *tree = hc->get_tree();
  BinaryTree<unsigned char> *node = tree;
  std::string *codebook = hc->get_codebook();
  for(int i=0; i<256; i++) {
    if(!codebook[i].empty()) {
      for(unsigned int j=0; j<codebook[i].length(); j++) {
        switch(codebook[i][j]) {
          case '0': ASSERT_NE(nullptr, node->left_child);
                    node = node->left_child;
                    break;
          case '1': ASSERT_NE(nullptr, node->right_child);
                    node = node->right_child;
                    break;
          default:  FAIL();
        }
      }
      EXPECT_EQ((unsigned char)i, node->value);
      node = tree;
    }
  }
}

TEST_F(HuffmanCodeTest, HuffmanTreeCanBeStoredAndRestored) {
  hc->encode(*iss, *ss);
  HuffmanCode hc2;
  hc2.decode(*ss, *oss);
  EXPECT_EQ(true, tree_equality(*(hc->get_tree()), *(hc2.get_tree())));
}

TEST_F(HuffmanCodeTest, CompressedDataCanBeRestored) {
  hc->encode(*iss, *ss);
  hc->decode(*ss, *oss);
  EXPECT_STREQ(iss->str().c_str(), oss->str().c_str());
}
