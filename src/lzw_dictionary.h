#pragma once

/// \brief Helper struct containing a sequence of bytes
struct ByteSequence {
  unsigned char *bytes; ///< Data (reverse order)
  unsigned int length;  ///< Length of data
};

/// \brief Optimized dictionary class for LZW coding.
/// Modelled on http://warp.povusers.org/EfficientLZW/index.html
class LZWDictionary {
public:
  /// \brief Constructor for dictionary with given constant codeword length
  /// \param n_bits The number of bits per codeword (dictionary size 2^n_bits-1)
  LZWDictionary(unsigned int n_bits);
  /// \brief Destructor. Frees all memory claimed by the dictionary.
  ~LZWDictionary();

  /// \brief Queries the dictionary for the index of a byte sequence
  /// \param prefix Index of the query sequence prefix (-1 for no null prefix)
  /// \param byte Final byte of the query sequence
  /// \return Index of the queried sequence or -1 if sequence is not in the dictionary
  int query(int prefix, unsigned char byte);
  /// \brief Queries the dictionary for the full byte sequence at a given index
  /// \param index Index of the byte sequence in the dictionary
  /// \return ByteSequence containing the data requested (note: length = 0, if the sequence is not in the dictionary)
  ByteSequence *query(int index);
  /// \brief Inserts a new string with the given prefix and final byte into the dictionary
  /// \param prefix Index of the sequence prefix (-1 for no null prefix)
  /// \param byte Final byte of the query sequence
  /// \return Index of the inserted sequence (or -1 if insertion failed)
  int insert(int prefix, unsigned char byte);
  /// \brief Resets the dictionary. After reset the dictionary contains the 256 single byte sequences.
  /// \return The reset code for the dictionary ((1 << n_bits) - 1)
  int reset();

  /// \brief Gets the current number of entries in the dictionary
  /// \return Number of entries (always greater than or equal to 256)
  unsigned int num_entries();
  /// \brief Gets the number of bits used to encode dictionary entries
  /// \return Number of bits
  unsigned int num_bits();

private:
  struct DictionaryEntry; ///< Forward declaration of DictionaryEntry structure

  unsigned int n_entries;   ///< Number of entries
  unsigned int n_bits;      ///< Number of bits/entry when encoding
  DictionaryEntry *entries; ///< Dictionary entries
  ByteSequence *prev_seq;   ///< Pre-allocated
};
