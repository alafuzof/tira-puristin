#pragma once

#include <iostream>

/// \brief This class provides a common interface to different code classes
class AbstractCode {
public:
  // \brief Virtual destructor
  virtual ~AbstractCode();

  /// \brief Encodes input to output
  /// \param input Input stream
  /// \param output Output stream
  /// \param verbose Enable verbose printing (default off)
  /// \param cout Output stream for verbose messages (default std::cout)
  /// \param cerr Output stream for error messages (default std::cerr)
  /// \return 0 for success, -1 for failure
  virtual int encode(std::istream &input, std::ostream &output,
                     bool verbose=false, std::ostream &cout=std::cout,
                     std::ostream &cerr=std::cerr) = 0;

  /// \brief Decodes Huffman coded data in input to output
  /// \param input Input stream
  /// \param output Output stream
  /// \param verbose Enable verbose printing (default off)
  /// \param cout Output stream for verbose messages (default std::cout)
  /// \param cerr Output stream for error messages (default  std::cerr)
  /// \return 0 for success, -1 for failure
  virtual int decode(std::istream &input, std::ostream &output,
                     bool verbose=false, std::ostream &cout=std::cout,
                     std::ostream &cerr=std::cerr) = 0;

  /// \brief Attempts to read the file header from input
  /// \param input Input stream
  /// \param cerr Output stream for error messages (default std::cerr)
  /// \return 0 for success, -1 for failure
  int read_header(std::istream &input, std::ostream &cerr=std::cerr);

  /// \brief Get unencoded length for the previous operation
  /// \return Unencoded length (in bytes)
  unsigned int get_unencoded_length();
  /// \brief Get encoded length for the previous operation
  /// \return Encoded length (in bytes)
  unsigned int get_encoded_length();

  /// \brief Get unencoded symbol count for the previous operation
  /// \return Unencoded symbol count
  unsigned int get_unencoded_symbol_count();
  /// \brief Get encoded symbol count for the previous operation
  /// \return Encoded symbol count
  unsigned int get_encoded_symbol_count();

  /// \brief Calculates the compression ratio for the previous opreation
  /// \return Compression ratio (input length / output_length)
  float get_compression_ratio();

  /// \brief Get total processing time for the previous operation
  /// \return Total time in seconds
  float get_total_time();
  /// \brief Get processing time per symbol for the previous operation
  /// \return Time per symbol in seconds
  float get_time_per_symbol();

protected:
  /// \brief Resets member variables on failed operation
  void operation_failed();

  // All the following are for the previous operation
  float total_time; ///< Total time elapsed
  unsigned int unencoded_symbol_count; ///< Unencoded symbol count
  unsigned int encoded_symbol_count; ///< Encoded symbol count
  unsigned int unencoded_length; ///< Unencoded length (in bytes)
  unsigned int encoded_length; ///< Encoded length for (in bytes)
};
