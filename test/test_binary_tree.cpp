#include "gtest/gtest.h"
#include "binary_tree.h"

class CharTreeTest : public ::testing::Test {
public:
  CharTreeTest() {
    leaf = new BinaryTree<char>();
    tree = new BinaryTree<char>('a', leaf, new BinaryTree<char>('c'));
  };
  ~CharTreeTest() {
    delete leaf;
    delete tree;
  };
  BinaryTree<char> *leaf;
  BinaryTree<char> *tree;
};

TEST_F(CharTreeTest, DefaultTreeIsLeaf) {
  EXPECT_EQ(true, leaf->leaf());
}

TEST_F(CharTreeTest, DefaultCharValueIsZero) {
  EXPECT_EQ('\0', leaf->value);
}

TEST_F(CharTreeTest, TreeStoresValueGivenOnInit) {
  BinaryTree<char> t('a');
  EXPECT_EQ('a', t.value);
}

TEST_F(CharTreeTest, TreeWithChildrenIsNotLeaf) {
  EXPECT_EQ(false, tree->leaf());
}

TEST_F(CharTreeTest, LeafCanBeStoredInBitStream) {
  leaf->value = 'b';
  std::ostringstream oss;
  BitWriter bw(oss);
  leaf->write(bw);
  bw.flush();
  EXPECT_EQ(2, oss.str().length());
  EXPECT_STREQ("b\0", oss.str().c_str());
}

TEST_F(CharTreeTest, TreeCanBeStoredInBitStream) {
  leaf->value = 'b';
  std::ostringstream oss;
  BitWriter bw(oss);
  tree->write(bw);
  bw.flush();
  EXPECT_EQ(4, oss.str().length());
  // Manually computed bit string: 0110 0001 1011 0001 0001 0110 0011 0000
  EXPECT_STREQ("\x61\xb1\x16\x30", oss.str().c_str());
}

TEST_F(CharTreeTest, LeafCanBeRestoredFromBitStream) {
  // \x01 used because otherwise stringstream assumes string has length 1
  std::istringstream iss("b\x01", std::ios_base::in|std::ios_base::binary);
  BitReader br(iss);
  leaf->read(br);
  EXPECT_EQ('b', leaf->value);
  EXPECT_EQ(true, leaf->leaf());
}

TEST_F(CharTreeTest, TreeCanBeRestoredFromBitStream) {
  // Here we restore a structure like tree above into leaf
  std::istringstream iss("\x61\xb1\x16\x30", std::ios_base::in|std::ios_base::binary);
  BitReader br(iss);
  leaf->read(br);
  EXPECT_EQ('a', leaf->value);
  EXPECT_EQ(false, leaf->leaf());
  EXPECT_EQ('b', leaf->left_child->value);
  EXPECT_EQ(true, leaf->left_child->leaf());
  EXPECT_EQ('c', leaf->right_child->value);
  EXPECT_EQ(true, leaf->right_child->leaf());
}

TEST_F(CharTreeTest, LeafSurvivesRoundTrip) {
  std::stringstream ss(std::ios_base::in|std::ios_base::out|std::ios_base::binary);
  BitWriter bw(ss);
  leaf->write(bw);
  BitReader br(ss);
  BinaryTree<char> bt('z', leaf, tree); // Note these are overwritten on read
  bt.read(br);
  EXPECT_EQ('\0', bt.value);
  EXPECT_EQ(true, bt.leaf());
}

TEST_F(CharTreeTest, TreeSurvivesRoundTrip) {
  std::stringstream ss(std::ios_base::in|std::ios_base::out|std::ios_base::binary);
  BitWriter bw(ss);
  tree->write(bw);
  bw.flush();
  BitReader br(ss);
  BinaryTree<char> bt;
  bt.read(br);
  EXPECT_EQ('a', bt.value);
  EXPECT_EQ(false, bt.leaf());
  EXPECT_EQ('\0', bt.left_child->value);
  EXPECT_EQ(true, bt.left_child->leaf());
  EXPECT_EQ('c', bt.right_child->value);
  EXPECT_EQ(true, bt.right_child->leaf());
}

TEST_F(CharTreeTest, IdenticalTreesAreEqual) {
  EXPECT_EQ(true, tree_equality(*leaf, *leaf));
  EXPECT_EQ(true, tree_equality(*tree, *tree));
}

TEST_F(CharTreeTest, DifferentTreesAreNotEqual) {
  EXPECT_EQ(false, tree_equality(*leaf, *tree));
  EXPECT_EQ(false, tree_equality(*tree, *leaf));
}
