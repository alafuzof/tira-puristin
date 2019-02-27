
#include "huffman_code.h"
#include "file_analyzer.h"
#include "priority_queue.h"


int HuffmanCode::build_code(std::istream &input) {
  // Calculate symbol frequencies
  FileAnalyzer fa;
  fa.analyze(input);
  raw_length = fa.length();
  if(raw_length == 0)
    return -1;
  unsigned int *freq = fa.frequencies();

  // Construct the Huffman tree
  tree = build_tree(freq);
  //delete[] freq; // No longer needed

  // Construct codebook
  codebook = build_codebook(*tree);

  return 0;
}

int HuffmanCode::encode(std::istream &input, std::ostream &output, bool verbose) {
  if(verbose) {std::cout << "Building Huffman code for input" << std::endl;}
  if(build_code(input) != 0) {
    std::cerr << "Failed to build Huffman code for input data!" << std::endl;
    return -1;
  }

  // Write file header
  if(verbose) {std::cout << "Writing file header" << std::endl;}
  if(write_header(output) != 0) {
    std::cerr << "Failed to write header!" << std::endl;
    return -1;
  }

  // Write symbols
  BitWriter bw(output);
  char c = '\0';
  if(verbose) {std::cout << "Starting to encode and write " << raw_length << " symbols" << std::endl;}
  for(unsigned int i=0; i<raw_length; i++) {
    input.get(c);
    bw.write_bitstring(codebook[(unsigned char)c]);
    //std::cout << "Writing character " << c << " bitstring " << codebook[(unsigned char)c] << std::endl;
  }
  bw.flush();
  if(verbose) {std::cout << "Done!" << std::endl;}

  return 0;
}

int HuffmanCode::decode(std::istream &input, std::ostream &output, bool verbose) {
  // Read file signature
  if(verbose) {std::cout << "Reading file header" << std::endl;}
  if(read_header(input) == -1) {
      std::cerr << "Header reading failed!" << std::endl;
      return -1;
  }

  // Reconstruct codebook
  codebook = build_codebook(*tree);
  if(verbose) {std::cout << "Reconstructed codebook:" << std::endl; print_codebook(codebook);}

  // Read symbols
  BitReader br(input);
  BinaryTree<unsigned char> *node = tree;
  if(verbose) {std::cout << "Starting to decode and write " << raw_length << " symbols" << std::endl;}
  unsigned int read_count = 0;
  while(read_count < raw_length) {
    bool bit = br.read_bit();
    // Move to left or right child depending on bit
    node = bit ? node->right_child : node->left_child;

    //  If we've arrived at a null child, something has gone wrong!
    if(node == nullptr) {
      std::cerr << "Encountered impossible bit during read!" << std::endl;
      return -1;
    }

    // If we're at a leaf, write out the symbol and restart from root
    if(node->leaf()) {
      output << node->value;
      read_count++;
      node = tree;
    }
  }
  if(verbose) {std::cout << "Done!" << std::endl;}

  return 0;
}

int HuffmanCode::read_header(std::istream &input) {
  BitReader br(input);

  // Read file format string
  std::string s = br.read_string();
  if(!s.compare("TIRA_PURISTIN HUFFMAN")) {
    std::cerr << "Incorrect format string: " << s << "! "
              << "(should be \"TIRA_PURISTIN HUFFMAN\")" << std::endl;
    return -1;
  }

  // Read raw (and encoded) length of message
  raw_length = (unsigned int)br.read_int();
  if((unsigned int)br.read_int() != raw_length) {
    std::cerr << "The number of encoded symbols should be equal to the number "
              << "of raw symbols in Huffman coded files!" << std::endl;
    return -1;
  }

  // Read Huffman tree and reconstruct codebook
  tree = new BinaryTree<unsigned char>();
  tree->read(br);
  br.flush();

  // Seek one character backwards
  input.seekg(-1, input.cur);

  return 0;
}

int HuffmanCode::write_header(std::ostream &output) {
  BitWriter bw(output);

  // Write file format string and raw (and encoded) length
  bw.write_string("TIRA_PURISTIN HUFFMAN");
  bw.write_int((int)raw_length);
  bw.write_int((int)raw_length); // Encoded == raw

  // Write Huffman tree
  if(tree != nullptr) {
    tree->write(bw);
    bw.flush();
  } else {
    std::cerr << "Huffman tree missing!" << std::endl;
    return -1;
  }

  return 0;
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
