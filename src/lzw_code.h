#pragma once

#include <iostream>
#include "bit_reader.h"
#include "bit_writer.h"
#include "lzw_dictionary.h"

class LZWCode {
public:
  LZWCode();
  ~LZWCode();

  void encode(std::istream &input, std::ostream &output, unsigned int n_bits);
  int decode(std::istream &input, std::ostream &output);

private:
  void write_header(BitWriter &bw);
  bool read_header(BitReader &br);

  unsigned int n_bits;
  unsigned int raw_length;
  unsigned int encoded_length;
  LZWDictionary *dictionary;
};
