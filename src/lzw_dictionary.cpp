#include <stdexcept>
#include <string>
#include "lzw_dictionary.h"

/// Helper struct for storing byte sequences in the dictionary
struct LZWDictionary::DictionaryEntry {
  int prefix_index;   ///< Index for the prefix of this sequence ("parent")
  unsigned char byte; ///< Final byte of the sequence
  int first_index;    ///< Index of the first sequence having this sequence as its prefix ("child")
  int next_index;     ///< Index of the next sequence sharing the same prefix as this one ("sibling")
};

LZWDictionary::LZWDictionary(unsigned int n_bits) {
  if(n_bits < 9 || n_bits > 20) {
    throw std::domain_error("Number of bits must be between 9 and 20");
  }
  this->n_bits = n_bits;
  entries = new LZWDictionary::DictionaryEntry[1 << n_bits];

  // A large shared ByteSequence is allocated to reduce allocations during queries
  prev_seq = new ByteSequence;
  prev_seq->length = 0;
  prev_seq->bytes = new unsigned char[1 << n_bits];
  reset();
}

LZWDictionary::~LZWDictionary() {
  delete[] entries;
  delete[] prev_seq->bytes;
  delete prev_seq;
}

int LZWDictionary::query(int prefix, unsigned char byte) {
  // If there is no prefix, the index is determined by the byte
  if(prefix == -1) {
    return (int)byte;
  }
  // Search through the "children" of this prefix
  int idx = entries[prefix].first_index; // First child
  while(idx != -1) {
    if(entries[idx].byte == byte) {
      return idx;
    }
    idx = entries[idx].next_index; // Next sibling
  }
  return -1;
}

ByteSequence *LZWDictionary::query(int index) {
  prev_seq->length = 0;
  int idx = index;
  int i = 0;
  // Start with the current index and reconstruct the sequence using the prefix
  // indices as "parent" links
  while(idx != -1) {
    // Note we place the bytes into the sequence in reverse order!
    prev_seq->bytes[i] = entries[idx].byte;
    prev_seq->length++;
    idx = entries[idx].prefix_index;
    i++;
  }

  // Note: we return the shared ByteSequence. The caller should not free this!
  return prev_seq;
}

int LZWDictionary::insert(int prefix, unsigned char byte) {
  // If trying to insert default symbol or the dictionary is full, return the null index
  if(n_entries >= (unsigned int)((1 << n_bits) - 1))
    return -1;

  // If trying to insert one of the default symbols, just return its index instead
  if(prefix == -1)
    return (unsigned int)byte;

  // If the prefix does not have a 'first' link, link the new entry there
  if(entries[prefix].first_index == -1) {
    entries[prefix].first_index = n_entries;
  }
  // Else follow the next chain of the prefix first and place the new entry
  // at the end
  else {
    int idx = entries[prefix].first_index;
    // If we encounter the string to be added, return its index
    if(entries[idx].byte == byte)
      return idx;

    while(entries[idx].next_index != -1) {
      idx = entries[idx].next_index;

      // If we encounter the string to be added, return its index
      if(entries[idx].byte == byte)
        return idx;
    }
    entries[idx].next_index = n_entries;
  }

  // Add the new entry
  entries[n_entries].prefix_index = prefix;
  entries[n_entries].byte = byte;
  entries[n_entries].first_index = -1;
  entries[n_entries].next_index = -1;

  return n_entries++;
}


int LZWDictionary::reset() {
  // During reset we re-insert the first 256 entries (all possible single byte
  // values)
  for(int i=0; i<256; i++) {
    entries[i].prefix_index = -1;
    entries[i].byte = (unsigned char)i;
    entries[i].first_index = -1;
    entries[i].next_index = -1;
  }
  n_entries = 256;

  return ((1 << n_bits)-1); // reset code
}

unsigned int LZWDictionary::num_bits() {
  return n_bits;
}

unsigned int LZWDictionary::num_entries() {
  return n_entries;
}
