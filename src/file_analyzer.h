#pragma once
#include <iostream>
#include <string>

/// This class analyzes the symbol counts of a given input
class FileAnalyzer {
public:
  /// Constructor. Reserves memory for file analysis.
  FileAnalyzer();
  /// Destructor. Releases reserved memory.
  ~FileAnalyzer();

  /// Counts how often different symbol occur in the input stream
  /// \param input Input stream
  void analyze(std::istream &input);
  /// Convenience function for analyzing a string instead of a stream
  /// \param input String of text to analyze
  void analyze(const std::string &input);

  /// Gets the number of symbols in the most recent input
  /// \return the number of analyzed symbols (or 0 before first analysis)
  unsigned int length();
  /// Calculates the entropy per symbol in the most recent input
  /// \return the number of entropy bits per symbol (or 0 before the first analysis)
  float entropy();
  /// Gets the symbol probabilities for the most recent input
  /// \return the probability of each symbol (including unseen symbols)
  float *probabilities();
  /// Get the symbol frequencies for the most recent input
  /// \return the frequency of each symbol (including unseen symbols)
  unsigned int *frequencies();

  /// Prints a summary of the analysis results on the previous input
  void print_report(std::ostream &output=std::cout);

private:
  unsigned int *m_symbol_count; ///< Symbol occurrence counts of most recent input
  unsigned int m_length; ///< Length of most recent input
  std::string *codebook; ///< Huffman codebook
};
