#include "bit_writer.h"

BitWriter::BitWriter(std::ostream &output) {
  output_stream = &output;
  // Initialize buffer and position
  buffer = 0;
  buffer_position = 0;
}

void BitWriter::write_bit(bool bit) {
  // Write bits from left to right
  if(bit)
    buffer |= (1 << (7-buffer_position));
  buffer_position++;
  // If the  buffer is full, flush it to the output stream
  if(buffer_position >= 8)
    flush();
}

void BitWriter::write_byte(unsigned char byte) {
  for(int i=0; i<8; i++) {
    write_bit(byte & (1<<(7-i)));
  }
}

void BitWriter::flush() {
  // Flushing writes the buffer to the stream and resets the buffer and position
  *output_stream << buffer;
  buffer = 0;
  buffer_position = 0;
}

BitWriter &BitWriter::operator<<(bool bit) {
  write_bit(bit);
  return *this;
}

BitWriter &BitWriter::operator<<(char byte) {
  write_byte(byte);
  return *this;
}