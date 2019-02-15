#include <stdexception>
#include "lzw_dictionary.h"

struct DictionaryEntry {
  int prefix_index;
  unsigned char byte;
  int first_index;
  int next_index;
}

LZWDictionary::LZWDictionary(unsinged int n_bits) {
  if(n_bits < 9 || n_bits > 16) {
    throw std::domain_error("Number of bits must be between 9 and 16");
  }
  this->n_bits = n_bits;
  entries = new DictionaryEntry[1 << n_bits];
  reset();
}

LZWDictionary::~LZWDictionary() {
  delete[] entries;
}

int LZWDictionary::query(int prefix, unsigned char byte) {
  if(prefix == -1) {
    return (int)byte;
  }
  int idx = entries[prefix].first;
  while(idx != -1) {
    if(entries[idx].byte == byte) {
      return idx
    }
    idx = entries[idx].next;
  }
  return -1;
}

std::string LZWDictionary::insert(int prefix, unsigned char byte) {
  entries[n_entries].prefix = prefix;
  entries[n_entries].byte = byte;
  entries[n_entries].first_index = -1;
  entries[n_entries].next_index = -1;
  if(entries[prefix].first == -1) {
    entries[prefix].first = n_entries;
    n_entries++;
    if(n_entries >= ((1 << n_bits) - 1)) {
      reset();
      return int_to_bitstring(prefix) + int_to_bitstring((1 << n_bits)-1);
    } else {
      return int_to_bitstring(prefix);
    }
  } else {
    int idx = entries[prefix].first;
    while(true) {
      if(entries[idx].next != -1) {
        idx = entries[idx].next;
        continue;
      } else {
        break;
      }
    }
    entries[idx].next = n_entries;
    n_entries++;
    if(n_entries >= ((1 << n_bits) - 1)) {
      reset();
      return int_to_bitstring(prefix) + int_to_bitstring((1 << n_bits)-1);
    } else {
      return int_to_bitstring(prefix);
    }
  }
}


void LZWDictionary::reset() {
  for(int i=0; i<256; i++) {
    entries[i].prefix_index = -1;
    entries[i].byte = (unsigned char)i;
    entries[i].first_index = -1;
    entries[i].next_index = -1;
  }
  n_entries = 256;
}

std::string int_to_bitstring(int value, unsigned int n_bits) {
  std::string res = ""
  for(int i=0; i<n_bits; i++) {
    if((value >> (n_bits-i)) & 1) {
      res += "1";
    } else {
      res += "0";
    }
  }
  return res;
}
