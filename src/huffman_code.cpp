
#include "huffman_code.h"
#include "file_analyzer.h"
#include "priority_queue.h"


void HuffmanCode::build_code(std::istream &input) {
  // Calculate symbol frequencies
  FileAnalyzer fa;
  fa.analyze(input);
  unsigned int *freq = fa.frequencies();

  // Construct the Huffman tree
  tree = build_tree(freq);
  delete[] freq; // No longer needed

  // Construct codebook
  codebook = build_codebook(*tree);
}

void HuffmanCode::encode(std::istream &input, std::ostream &output) {
  char c;
  input >> c;
  output << c;
}

void HuffmanCode::decode(std::istream &input, std::ostream &output) {
  char c;
  input >> c;
  output << c;
}

BinaryTree<unsigned char> *HuffmanCode::get_tree() {
  return tree;
}

std::string *HuffmanCode::get_codebook() {
  return codebook;
}

BinaryTree<unsigned char> *build_tree(unsigned int *frequencies) {
  PriorityQueue<BinaryTree<unsigned char>*> pq(256, MIN_PRIORITY);
  // Initialize queue with leaf nodes
  for(int i=0; i<256; i++) {
    if(frequencies[i] > 0) {
      pq.push(new BinaryTree<unsigned char>((unsigned char)i), frequencies[i]);
    }
  }
  // Combine subtrees in increasing order of occurrence frequency§
  while(pq.length() > 1) {
    Element<BinaryTree<unsigned char>*> left = pq.pop();
    Element<BinaryTree<unsigned char>*> right = pq.pop();
    pq.push(new BinaryTree<unsigned char>('\0', left.value, right.value), left.priority+right.priority);
  }
  // The final Element is the root of the tree
  if(!pq.empty())
    return pq.pop().value;
  else
    return nullptr;
}

struct TreePathPair {
  BinaryTree<unsigned char> tree;
  std::string path;
  TreePathPair() {}
  TreePathPair(BinaryTree<unsigned char> t, std::string p): tree(t), path(p) {};
};

std::string *build_codebook(BinaryTree<unsigned char> root) {
  std::string *codebook = new std::string[256];
  for(int i=0; i<256; i++) {
    codebook[i] = "";
  }

  PriorityQueue<TreePathPair> pq(512, MIN_PRIORITY);
  pq.push(TreePathPair(root, ""), 1);
  while(!pq.empty()) {
    TreePathPair pair = pq.pop().value;
    if(pair.tree.leaf() && pair.path.empty()) // Special case root leaf node
      codebook[pair.tree.value] = "0";
    if(pair.tree.leaf() && !pair.path.empty())
      codebook[pair.tree.value] = pair.path;
    if(pair.tree.left_child != nullptr)
      pq.push(TreePathPair(*pair.tree.left_child, pair.path + "0"), 1);
    if(pair.tree.right_child != nullptr)
      pq.push(TreePathPair(*pair.tree.right_child, pair.path + "1"), 1);
  }

  return codebook;
}
