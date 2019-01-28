#pragma once

#include <iostream>

/// Simple I/O for reading single bits and unaligned bytes
class BitReader {
public:
  /// \brief Constructor. Initializes object for reading from an input stream.
  /// \param input Input stream for reading
  BitReader(std::istream &input);

  /// \brief Read a bit from the current bit position in the input stream.
  /// \return Boolean indicating the bit value
  bool read_bit();
  /// \brief Read a byte starting from the current bit position in the input stream.
  /// \return Byte as read from the input stream
  unsigned char read_byte();

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
