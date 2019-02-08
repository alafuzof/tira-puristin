#pragma once

#include <iostream>

/// Simple I/O for writing single bits and unaligned bytes
class BitWriter {
public:
  /// \brief Constructor. Initializes object for writing to an output stream
  /// \param output Output stream for writing
  BitWriter(std::ostream &output);

  /// \brief Write a bit to the current position in the buffer or output stream
  /// \param bit The bit to be written
  void write_bit(bool bit);
  /// \brief Write a byte to the current position in the buffer or output stream
  /// \param byte The byte to be written
  void write_byte(unsigned char byte);

  void write_string(std::string str);
  void write_int(int i);

  /// \brief Flush the buffer to the output stream filling with zeros if necessary
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
