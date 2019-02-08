#include <sstream>
#include "bit_reader.h"

BitReader::BitReader(std::istream &input) {
  input_stream = &input;
  // Initialize buffer and position
  *input_stream >> buffer;
  buffer_position = 0;
}

bool BitReader::read_bit() {
  // If we should be reading bits from the next byte, read a new byte into the
  // buffer and reset position
  if(buffer_position >= 8)
    flush();
  // Read bits from left to right
  bool bit = (buffer & (1 << (7-buffer_position)));
  buffer_position++;
  return bit;
}

unsigned char BitReader::read_byte() {
  unsigned char byte = 0;
  for(int i=0; i<8; i++) {
    if(read_bit()) // Only set those bits set in input
      byte |= (1 << (7-i));
  }
  return byte;
}

std::string BitReader::read_string() {
  std::ostringstream oss;
  while(true) {
    unsigned char byte = read_byte();
    oss << byte;
    if(byte == '\0')
      break;
  }
  return oss.str();
}

int BitReader::read_int() {
  int x = 0;
  unsigned char *xp = (unsigned char*)&x;
  for(int i=0; i<4; i++) {
    xp[3-i] = read_byte(); // Little-endian
  }
  return x;
}

void BitReader::flush() {
  *input_stream >> buffer;
  buffer_position = 0;
}

BitReader &BitReader::operator>>(bool &bit) {
  bit = read_bit();
  return *this;
}

BitReader &BitReader::operator>>(unsigned char &byte) {
  byte = read_byte();
  return *this;
}
