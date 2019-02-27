#pragma once

#include <iostream>
#include "bit_reader.h"
#include "bit_writer.h"
#include "lzw_dictionary.h"

/// This class encodes/decodes arbitrary data using LZW coding
class LZWCode {
public:
  /// Default constructor.
  LZWCode();
  /// Destructor. Frees any memory reserved by the object.
  ~LZWCode();

  /// Encodes input to output using LZW encoding
  /// \param input Input stream
  /// \param output Output stream
  /// \parma n_bits Encoding bitrate
  void encode(std::istream &input, std::ostream &output, unsigned int n_bits);
  /// Decodes LZW coded data in input to output
  /// \param input Input stream
  /// \param output Output stream
  /// \return 0 for success, -1 for failure
  int decode(std::istream &input, std::ostream &output);

  /// Attempts to read the Huffman coding file header from input
  /// \param input Input stream
  /// \return 0 for success, -1 for failure
  int read_header(std::istream &input);

private:
  /// Attempts to write the Huffman coding file header to output
  /// \param output Output stream
  void write_header(BitWriter &bw);


  unsigned int n_bits; ///< LZW bitrate (dictionary size) for most recent operation§
  unsigned int raw_length; ///< Unenecoded symbol count for most recent operation
  unsigned int encoded_length; ///< Encoded symbol count for most recent operation
  LZWDictionary *dictionary; ///< LZW dictionary for most recent operation
};
