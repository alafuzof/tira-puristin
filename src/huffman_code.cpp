
#include "huffman_code.h"
#include "file_analyzer.h"
#include "priority_queue.h"


void HuffmanCode::build_code(std::istream &input) {
  // Calculate symbol frequencies
  FileAnalyzer fa;
  fa.analyze(input);
  m_length = fa.length();
  unsigned int *freq = fa.frequencies();

  // Construct the Huffman tree
  tree = build_tree(freq);
  delete[] freq; // No longer needed

  // Construct codebook
  codebook = build_codebook(*tree);
}

void HuffmanCode::encode(std::istream &input, std::ostream &output, bool verbose) {
  if(verbose) {std::cout << "Building Huffman code for input" << std::endl;}
  build_code(input);
  if(verbose) {std::cout << "Constructed codebook:" << std::endl; print_codebook(codebook);}

  BitWriter bw(output);
  if(verbose) {std::cout << "Writing file signature" << std::endl;}
  bw.write_string("TIRA_PURISTIN");
  if(verbose) {std::cout << "Writing Huffman tree" << std::endl;}
  //tree->print_tree();
  tree->write(bw);
  bw.flush();
  //return;
  if(verbose) {std::cout << "Writing symbol count" << std::endl;}
  bw.write_int((int)m_length);
  char c = '\0';
  if(verbose) {std::cout << "Starting to encode and write " << m_length << " symbols" << std::endl;}
  for(unsigned int i=0; i<m_length; i++) {
    input.get(c);
    bw.write_bitstring(codebook[(unsigned char)c]);
    //std::cout << "Writing character " << c << " bitstring " << codebook[(unsigned char)c] << std::endl;
  }
  bw.flush();
  if(verbose) {std::cout << "Done!" << std::endl;}
}

void HuffmanCode::decode(std::istream &input, std::ostream &output, bool verbose) {
  BitReader br(input);
  if(verbose) {std::cout << "Reading file signature" << std::endl;}
  std::string s = br.read_string();
  if(!s.compare("TIRA_PURISTIN")) {
    std::cerr << "The given input is not a puristin compressed file:" << std::endl;
    std::cerr << "Initial string: " << s << " (should be TIRA_PURISTIN)" << std::endl;
    return;
  }

  if(verbose) {std::cout << "Reading Huffman tree" << std::endl;}
  tree = new BinaryTree<unsigned char>();
  tree->read(br);
  br.flush();
  codebook = build_codebook(*tree);
  if(verbose) {std::cout << "Reconstructed codebook:" << std::endl; print_codebook(codebook);}

  //tree->print_tree();
  if(verbose) {std::cout << "Reading symbol count" << std::endl;}
  m_length = (unsigned int)br.read_int();
  BinaryTree<unsigned char> *node = tree;
  if(verbose) {std::cout << "Starting to decode and write " << m_length << " symbols" << std::endl;}
  unsigned int i = 0;
  //std::cout << "Reading bits: ";
  while(i < m_length) {
    bool bit = br.read_bit();
    if((bit && node->left_child == nullptr) || (!bit && node->right_child == nullptr)) {
      std::cerr << "Encountered impossible bit during read" << std::endl;
      return;
    }
    if(!bit) {
      //std::cout << "0";
      node = node->left_child;
    } else {
      //std::cout << "1";
      node = node->right_child;
    }

    if(node->leaf()) {
      //std::cout << " decoded: " << node->value << "\n" << "Reading bits: ";
      output << node->value;
      node = tree;
      i++;
    }
  }
  if(verbose) {std::cout << "Done!" << std::endl;}
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

void print_codebook(std::string *codebook, std::ostream &output) {
  for(int i=0; i<256; i++) {
    if(!codebook[i].empty()) {
      if(i < 33 || i > 126)
        output << i << "\t \t" << codebook[i] << std::endl;
      else
        output << i << "\t" << (char)i << "\t" << codebook[i] << std::endl;
    }
  }
}
