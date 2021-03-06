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
  // Bytes are written one bit at a time, this could probably be improved
  for(int i=0; i<8; i++) {
    write_bit(byte & (1<<(7-i)));
  }
}

void BitWriter::write_string(std::string str) {
  for(unsigned int i=0; i<str.length(); i++) {
    write_byte((unsigned char)str[i]);
  }
  write_byte('\0');
}

void BitWriter::write_bitstring(std::string bstr) {
  for(unsigned int i=0; i<bstr.length(); i++) {
    switch(bstr[i]) {
      case '0': write_bit(false);
                break;
      case '1': write_bit(true);
                break;
      case ' ': break;
      default:  throw std::logic_error("Bit string contained bad character!");
    }
  }
}

void BitWriter::write_int(int i) {
  unsigned char *ip = (unsigned char *)&i;
  // Integers are written one byte at a time, this could probably be improved
  for(int j=0; j<4; j++) {
    write_byte(ip[3-j]); // Little-endian
  }
}

void BitWriter::write_uint(unsigned int i, unsigned int n_bits) {
  // Integers are written one bit at a time, this could probably be improved
  for(int j=n_bits-1; j>=0; j--) {
    write_bit((i>>j) & 1);
  }
}

void BitWriter::flush() {
  // Flushing writes the buffer to the stream and resets the buffer and position
  if(buffer_position != 0) {
    *output_stream << buffer;
    buffer = 0;
    buffer_position = 0;
  }
}

BitWriter &BitWriter::operator<<(bool bit) {
  write_bit(bit);
  return *this;
}

BitWriter &BitWriter::operator<<(char byte) {
  write_byte(byte);
  return *this;
}
