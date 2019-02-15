#pragma once

struct DictionaryEntry;

// Optimized dictionary class for LZW coding
// Modelled on http://warp.povusers.org/EfficientLZW/index.html
class LZWDictionary {
public:
  // Constructor for dictionary with n_bits codeword length
  LZWDictionary(unsigned int n_bits);
  // Desctructor
  ~LZWDictionary();

  // Queries the dictionary for a string with the given prefix and final byte
  int query(int prefix, unsigned char byte);
  // Inserts a new string with the given prefix and final byte into the dictionary
  std::string insert(int prefix, unsigned char byte);
  // Resets the dictionary
  void reset();

private:
  unsigned int n_entries;
  unsigned int n_bits;
  DictionaryEntry *entries;
};

// Produces a bit representation of the given int at the given number of bits
std::string int_to_bitstring(int value, unsigned int n_bits);
