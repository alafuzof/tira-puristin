#include <cmath>
#include "abstract_code.h"

AbstractCode::~AbstractCode() {}

unsigned int AbstractCode::get_unencoded_length() {
  return unencoded_length;
}

unsigned int AbstractCode::get_encoded_length() {
  return encoded_length;
}

unsigned int AbstractCode::get_unencoded_symbol_count() {
  return unencoded_symbol_count;
}

unsigned int AbstractCode::get_encoded_symbol_count() {
  return encoded_symbol_count;
}

float AbstractCode::get_compression_ratio() {
  if(total_time == NAN || encoded_length == 0)
    return NAN;

  return ((float)unencoded_length) / encoded_length;
}

float AbstractCode::get_total_time() {
  return total_time;
}

float AbstractCode::get_time_per_symbol() {
  if(total_time == NAN)
    return NAN;

  if(encoded_symbol_count != 0)
    return total_time / unencoded_symbol_count;

  return 0;
}

void AbstractCode::operation_failed() {
  total_time = NAN;
  unencoded_length = 0;
  encoded_length = 0;
  unencoded_symbol_count = 0;
  encoded_symbol_count = 0;
}
