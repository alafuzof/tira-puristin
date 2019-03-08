#pragma once

#include <iostream>

/// \brief Simple I/O for reading single bits and unaligned bytes.
///
/// All reads start at the current bit position of the BitReader and advance the
/// position by the number of bits read.
class BitReader {
public:
  /// \brief Constructor. Initializes object for reading from an input stream.
  /// \param input Input stream for reading
  BitReader(std::istream &input);

  /// \brief Read a bit
  /// \return Boolean indicating the bit value
  bool read_bit();
  /// \brief Read a byte
  /// \return Byte value as read from the input stream
  unsigned char read_byte();
  /// \brief Read a null-terminated character string
  /// \return String read from the input stream
  std::string read_string();
  /// \brief Read a (signed 4-byte) integer
  /// \return Integer value read from the input stream
  int read_int();
  /// \brief Read an unsigned variable length integer
  /// \return Unsigned integer value read from the input stream
  unsigned int read_uint(unsigned int n_bits);
  /// \brief Flush the reader.
  /// Advances the BitReader position to the next byte boundary.
  void flush();

  /// \brief Convenience "stream operator" for reading bits
  /// \param bit Boolean indicating the bit value
  /// \return The BitReader (this) for chaining reads
  BitReader &operator>>(bool &bit);
  /// \brief Convenience "stream operator" for reading bytes
  /// \param bit Byte as read from the input stream
  /// \return The BitReader (this) for chaining reads
  BitReader &operator>>(unsigned char &byte);

private:
  std::istream *input_stream; ///< Input stream (string stream, file...)
  unsigned char buffer; ///< Buffer for interfacing with byte based stream
  int buffer_position;  ///< Starting bit position for the next read
};
