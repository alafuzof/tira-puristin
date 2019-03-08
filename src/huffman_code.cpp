#include <ctime>
#include "huffman_code.h"
#include "file_analyzer.h"
#include "priority_queue.h"

HuffmanCode::~HuffmanCode() {}

int HuffmanCode::build_code(std::istream &input) {
  // Calculate symbol frequencies
  FileAnalyzer fa;
  fa.analyze(input);
  encoded_symbol_count = unencoded_symbol_count = fa.length();

  if(unencoded_symbol_count == 0)
    return -1;
  unsigned int *freq = fa.frequencies();

  // Construct the Huffman tree
  tree = build_tree(freq);

  // Construct codebook
  codebook = build_codebook(*tree);

  return 0;
}

int HuffmanCode::encode(std::istream &input, std::ostream &output, bool verbose,
                        std::ostream &cout, std::ostream &cerr) {
  // Measuring performance (start)
  clock_t t1 = clock();
  std::streampos output_start = output.tellp();

  // Build Huffman code
  if(verbose) {cout << "Building Huffman code for input" << std::endl;}
  if(build_code(input) != 0) {
    cerr << "Failed to build Huffman code for input data!" << std::endl;
    operation_failed();
    return -1;
  }

  // Write file header
  if(verbose) {cout << "Writing file header" << std::endl;}
  if(write_header(output, cerr) != 0) {
    cerr << "Failed to write header!" << std::endl;
    operation_failed();
    return -1;
  }

  // Write symbols
  BitWriter bw(output);
  char c = '\0';
  if(verbose) {cout << "Starting to encode and write " << unencoded_symbol_count << " symbols" << std::endl;}
  for(unsigned int i=0; i<unencoded_symbol_count; i++) {
    input.get(c);
    bw.write_bitstring(codebook[(unsigned char)c]);
  }
  bw.flush();
  if(verbose) {cout << "Done!" << std::endl;}

  // Measure performance (stop)
  clock_t t2 = clock();
  total_time = (((float)(t2-t1))/CLOCKS_PER_SEC);
  std::streampos output_stop = output.tellp();
  encoded_length = output_stop-output_start;
  unencoded_length = unencoded_symbol_count;

  return 0;
}

int HuffmanCode::decode(std::istream &input, std::ostream &output, bool verbose,
                        std::ostream &cout, std::ostream &cerr) {
  // Measuring performance (start)
  clock_t t1 = clock();
  std::streampos input_start = input.tellg();

  // Read file signature
  if(verbose) {cout << "Reading file header" << std::endl;}
  if(read_header(input, cerr) == -1) {
      cerr << "Header reading failed!" << std::endl;
      operation_failed();
      return -1;
  }

  // Reconstruct codebook
  codebook = build_codebook(*tree);
  if(verbose) {cout << "Reconstructed codebook:" << std::endl; print_codebook(codebook, cout);}

  // Read symbols
  if(verbose) {cout << "Starting to decode " << encoded_symbol_count << " symbols" << std::endl;}
  BitReader br(input);
  BinaryTree<unsigned char> *node = tree;
  unsigned int read_count = 0;
  while(read_count < encoded_symbol_count) {
    bool bit = br.read_bit();
    // Move to left or right child depending on bit
    node = bit ? node->right_child : node->left_child;

    //  If we've arrived at a null child, something has gone wrong!
    if(node == nullptr) {
      cerr << "Encountered impossible bit during read!" << std::endl;
      operation_failed();
      return -1;
    }

    // If we're at a leaf, write out the symbol and restart from root
    if(node->leaf()) {
      output << node->value;
      read_count++;
      node = tree;
    }
  }

  if(verbose) {cout << "Done!" << std::endl;}

  // Measure performance (stop)
  clock_t t2 = clock();
  total_time = (((float)(t2-t1))/CLOCKS_PER_SEC);
  std::streampos input_stop = input.tellg();
  encoded_length = input_stop-input_start;
  unencoded_length = unencoded_symbol_count;

  return 0;
}

int HuffmanCode::read_header(std::istream &input, std::ostream &cerr) {
  BitReader br(input);

  // Read file format string
  std::string s = br.read_string();
  if(!s.compare("TIRA_PURISTIN HUFFMAN")) {
    cerr << "Incorrect format string: " << s << "! "
         << "(should be \"TIRA_PURISTIN HUFFMAN\")" << std::endl;
    return -1;
  }

  // Read raw (and encoded) length of message
  unencoded_symbol_count = (unsigned int)br.read_int();
  encoded_symbol_count = (unsigned int)br.read_int();
  if(unencoded_symbol_count != encoded_symbol_count) {
    cerr << "The number of encoded symbols should be equal to the number "
         << "of unencoded symbols in Huffman coded files!" << std::endl;
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

int HuffmanCode::write_header(std::ostream &output, std::ostream &cerr) {
  BitWriter bw(output);

  // Write file format string and raw (and encoded) length
  bw.write_string("TIRA_PURISTIN HUFFMAN");
  bw.write_int((int)unencoded_symbol_count);
  bw.write_int((int)unencoded_symbol_count); // Encoded == unencoded

  // Write Huffman tree
  if(tree != nullptr) {
    tree->write(bw);
    bw.flush();
  } else {
    cerr << "Huffman tree missing!" << std::endl;
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

/// \brief Convenience struct for storing a tree and the path that led to it
struct TreePathPair {
  BinaryTree<unsigned char> tree; ///< Current root
  std::string path; ///< The path of right and left children that led to the current root

  /// \brief Default constructor
  TreePathPair() {}
  /// \brief Constructor
  /// \param t The current tree root
  /// \param p The path taken to get to the t (encoded as '0' for left subtree and '1' for right subtree)
  TreePathPair(BinaryTree<unsigned char> t, std::string p): tree(t), path(p) {};
};

std::string *build_codebook(BinaryTree<unsigned char> root) {
  // Initialize codebook with empty strings
  std::string *codebook = new std::string[256];
  for(int i=0; i<256; i++) {
    codebook[i] = "";
  }

  // Perform breadth-first traversal of the tree and update the codebook at each
  // leaf node
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
      // ASCII values <33 or >126 are not printable, so we only print the code
      if(i < 33 || i > 126)
        output << i << "\t \t" << codebook[i] << std::endl;
      // For the printable characters, print the character as well
      else
        output << i << "\t" << (char)i << "\t" << codebook[i] << std::endl;
    }
  }
}
