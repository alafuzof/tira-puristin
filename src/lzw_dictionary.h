#pragma once

struct DictionaryEntry;

struct ByteSequence {
  unsigned char *bytes;
  unsigned int length;
};

// Optimized dictionary class for LZW coding
// Modelled on http://warp.povusers.org/EfficientLZW/index.html
class LZWDictionary {
public:
  // Constructor for dictionary with n_bits codeword length
  LZWDictionary(unsigned int n_bits);
  // Desctructor
  ~LZWDictionary();

  // Queries the dictionary for a byte sequence with the given prefix and final byte
  int query(int prefix, unsigned char byte);
  // Queries the dictionary for the full sequence ending at the character at a given index
  ByteSequence *query(int index);
  // Inserts a new string with the given prefix and final byte into the dictionary
  int insert(int prefix, unsigned char byte);
  // Resets the dictionary
  int reset();

  unsigned int num_entries();
  unsigned int num_bits();

private:
  unsigned int n_entries;
  unsigned int n_bits;
  DictionaryEntry *entries;
  ByteSequence *prev_seq;
};
