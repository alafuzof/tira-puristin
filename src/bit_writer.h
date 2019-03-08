#pragma once

#include <iostream>

/// \brief Simple I/O for writing single bits and unaligned bytes.
///
/// All writes start at the current bit position of the BitWriter and advance
/// the position by the number of bits written.
class BitWriter {
public:
  /// \brief Constructor. Initializes object for writing to an output stream
  /// \param output Output stream for writing
  BitWriter(std::ostream &output);

  /// \brief Write a bit
  /// \param bit The bit to be written
  void write_bit(bool bit);
  /// \brief Write a byte
  /// \param byte The byte to be written
  void write_byte(unsigned char byte);
  /// \brief Write a character string
  /// \param str The string to be written
  void write_string(std::string str);
  /// \brief Write a sequence of bits
  /// \param bstr A string containing '0' and '1' characters corresponding to bit values
  void write_bitstring(std::string bstr);
  /// \brief Write a signed 4-byte integer
  /// \param i The integer to be written
  void write_int(int i);
  /// \brief Write an unsigned variable length integer
  /// \param i The integer value to be written
  /// \param n_bits The number of bits used for the representation of i
  void write_uint(unsigned int i, unsigned int n_bits);

  /// \brief Flush the buffer to the output stream.
  /// Advances the BitWriter position to the next byte boundary writing zeros on the way.
  void flush();

  /// \brief Convenience "stream operator" for writing bits
  /// \param bit The bit to be written
  /// \return The BitWriter (this) for chaining writes
  BitWriter &operator<<(bool bit);
  /// \brief Convenience "stream operator" for writing bytes
  /// \param byte The byte to be written
  /// \return The BitWriter (this) for chaining writes
  BitWriter &operator<<(char byte);

private:
  std::ostream *output_stream; ///< Output stream (string stream, file...)
  unsigned char buffer; ///< Buffer for interfacing with byte based stream
  int buffer_position;  ///< Starting bit position for the next write
};
