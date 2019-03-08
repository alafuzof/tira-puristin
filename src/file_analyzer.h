#pragma once
#include <iostream>
#include <string>

/// \brief This class analyzes the symbol counts of a given input
class FileAnalyzer {
public:
  /// \brief Constructor. Reserves memory for file analysis.
  FileAnalyzer();
  /// \brief Destructor. Releases reserved memory.
  ~FileAnalyzer();

  /// \brief Counts how often different symbol occur in the input stream.
  /// Restores the original stream position.
  /// \param input Input stream
  void analyze(std::istream &input);
  /// \brief Convenience function for analyzing a string instead of a stream
  /// \param input String of text to analyze
  void analyze(const std::string &input);

  /// \brief Gets the number of symbols in the most recent input
  /// \return the number of analyzed symbols (or 0 before first analysis)
  unsigned int length();
  /// \brief Calculates the entropy per symbol in the most recent input
  /// \return the number of entropy bits per symbol (or 0 before the first analysis)
  float entropy();
  /// \brief Gets the symbol probabilities for the most recent input
  /// \return the probability of each symbol (including unseen symbols)
  float *probabilities();
  /// \brief Get the symbol frequencies for the most recent input
  /// \return the frequency of each symbol (including unseen symbols)
  unsigned int *frequencies();


private:
  unsigned int *symbol_count; ///< Symbol occurrence counts of most recent input
  unsigned int input_length; ///< Length of most recent input
};
