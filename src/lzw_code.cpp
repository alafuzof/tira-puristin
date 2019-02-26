#include <iomanip>
#include <string>
#include "file_analyzer.h"
#include "lzw_code.h"

void write_bytesequence(std::ostream &output, ByteSequence *bs) {
  if(bs->length == 0)
    return;
  for(int i=bs->length-1; i>=0; i--) {
    output.put(bs->bytes[i]);
  }
}

std::string seq2str(ByteSequence *bs) {
  if(bs->length == 0)
    return "";
  std::string res = "\"";
  for(int i=bs->length-1; i>=0; i--) {
    if(bs->bytes[i] == 10)
      res += "\\n";
    else if(bs->bytes[i] == 13)
      res += "\\r";
    else
      res += bs->bytes[i];
  }
  res += "\"";
  return res;
}

LZWCode::LZWCode(): n_bits(0), raw_length(0), encoded_length(0), dictionary(nullptr) {}

LZWCode::~LZWCode() {
  if(dictionary != nullptr)
    delete dictionary;
  dictionary = nullptr;
}

void LZWCode::encode(std::istream &input, std::ostream &output, unsigned int n_bits) {
  // Get file length from analyzer
  FileAnalyzer fa;
  fa.analyze(input);
  raw_length = fa.length();

  // Initialize dictionary
  this->n_bits = n_bits;
  if(dictionary != nullptr)
    delete dictionary;
  dictionary = new LZWDictionary(n_bits);

  // Initialize output writer
  BitWriter bw(output);

  // Write file header
  write_header(bw);

  // Write symbols
  int prefix = -1;
  char c = '\0';
  encoded_length = 0;
  // std::cout << "Char  Prefix     Full     Found   Added   Wrote" << std::endl;
  for(unsigned int i=0; i<raw_length; i++) {
    input.get(c);
    int index = dictionary->query(prefix, (unsigned char)c);
    if(index != -1) { // Query found in dictionary
      // std::string d = "a";
      // if(c == 10)
      //   d = "\\n";
      // else if (c == 13)
      //   d = "\\r";
      // else
      //   d[0] = c;
      // std::cout << std::setw(3) << d << "    "
      //           << std::setw(5) << seq2str(dictionary->query(prefix)) << "   "
      //           << std::setw(6) << seq2str(dictionary->query(index))
      //           << "       1       (-)     (-)" << std::endl;
      prefix = index;
    } else {          // Query not found in dictionary
      bw.write_uint(prefix, n_bits);
      encoded_length++;

      int new_index = dictionary->insert(prefix, (unsigned char)c);
      // std::string d = "a";
      // if(c == 10)
      //   d = "\\n";
      // else if (c == 13)
      //   d = "\\r";
      // else
      //   d[0] = c;
      // std::cout << std::setw(3) << d << "    "
      //           << std::setw(5) << seq2str(dictionary->query(prefix)) << "   "
      //           << std::setw(6) << seq2str(dictionary->query(new_index))
      //           << "            "
      //           << std::setw(6) << seq2str(dictionary->query(new_index)) << "    "
      //           << std::setw(4) << prefix << std::endl;
      if(new_index == -1) {
        //std::cout << "Reset!" << std::endl;
        bw.write_uint(dictionary->reset(), n_bits);
        encoded_length++;
      }

      prefix = dictionary->query(-1, (unsigned char)c);
    }
  }
  bw.write_uint(prefix, n_bits);
  encoded_length++;
  bw.flush();
  output.seekp(18, std::ios_base::beg); // Seek to encoded count in file
  bw.write_int((int)encoded_length);
}

int LZWCode::decode(std::istream &input, std::ostream &output) {
  // Initialize input reader
  BitReader br(input);

  // Read header
  if(!read_header(br)) {
    std::cerr << "Aborting decode!" << std::endl;
    return -1;
  }

  // Initialize dictionary
  if(dictionary != nullptr)
    delete dictionary;
  dictionary = new LZWDictionary(n_bits);

  // Read encoded symbols
  size_t start = output.tellp();
  int index = br.read_uint(n_bits);
  unsigned int read_symbols = 1;
  ByteSequence *bs = dictionary->query(index);
  write_bytesequence(output, bs);
  int old = index;
   // std::cout << "Index    IStr      Old  Found    Wrote    Added" << std::endl;
   // std::cout << seq2str(dictionary->query(index)) << " " << seq2str(bs) << std::endl;
   // std::cout << std::setw(5) << index << "   "
   //          << std::setw(6) << seq2str(dictionary->query(index)) << "   "
   //          << std::setw(6) << seq2str(dictionary->query(old)) << "    1     "
   //          << std::setw(6) << seq2str(bs) << std::endl;
  while(read_symbols < encoded_length) {
    index = br.read_uint(n_bits);
    read_symbols++;
    if(index == ((1 << n_bits)-1)) {
      dictionary->reset();
      if(read_symbols < encoded_length) {
        index = br.read_uint(n_bits);
        read_symbols++;
        bs = dictionary->query(index);
        write_bytesequence(output, bs);
        old = index;
      }
      continue;
    }
    if((unsigned int)index < dictionary->num_entries()) { // Index is in the dictionary
      bs = dictionary->query(index);
      write_bytesequence(output, bs);
      // int new_insert = dictionary->insert(old, bs->bytes[bs->length-1]);
      dictionary->insert(old, bs->bytes[bs->length-1]);
       // std::cout << std::setw(5) << index << "   "
       //           << std::setw(6) << seq2str(dictionary->query(index)) << "   "
       //           << std::setw(6) << seq2str(dictionary->query(old)) <<  "    1     "
       //           << std::setw(6) << seq2str(dictionary->query(index)) << "   "
       //           << std::setw(6) << seq2str(dictionary->query(new_insert)) << std::endl;
    } else { // Index is not in the dictionary
      bs = dictionary->query(old);
      // int new_insert = dictionary->insert(old, bs->bytes[bs->length-1]);
      dictionary->insert(old, bs->bytes[bs->length-1]);
      write_bytesequence(output, bs);
      output.put(bs->bytes[bs->length-1]);
      // std::string d = "a";
      // if(bs->bytes[0] == 10)
      //   d = "\\n";
      // else if (bs->bytes[0] == 13)
      //   d = "\\r";
      // else
      //   d[0] = bs->bytes[0];
      //  std::cout << std::setw(5) << index << " "
      //            << std::setw(6) <<  " (-)  "
      //            << std::setw(6) << seq2str(dictionary->query(old)) << "         "
      //            << std::setw(6) << seq2str(dictionary->query(old)) << "   "
      //            << std::setw(6) << seq2str(dictionary->query(new_insert)) << std::endl;
    }
    old = index;
  }
  if((size_t)output.tellp() - start != raw_length) {
    std::cerr << "Expected " << raw_length << " symbols in decoded message "
              << "but saw " << (size_t)output.tellp() - start << "symbols!" << std::endl;
    return -1;
  }
  return 0;
}

void LZWCode::write_header(BitWriter &bw) {
  bw.write_string("TIRA_PURISTIN");
  bw.write_int((int)raw_length); // Raw symbol count
  bw.write_int(0); // Encoded symbol count
  bw.write_string("LZW");
  bw.write_byte((unsigned char)n_bits);
}

bool LZWCode::read_header(BitReader &br) {
  std::string s = br.read_string();
  if(!s.compare("TIRA_PURISTIN")) {
    std::cerr << "Incorrect file identifier: " << s << " (should be TIRA_PURISTIN)" << std::endl;
    return false;
  }
  raw_length = (unsigned int)br.read_int();
  encoded_length = (unsigned int)br.read_int();
  s = br.read_string();
  if(!s.compare("LZW")) {
    std::cerr << "Incorrect format string: " << s << " (should be LZW)" << std::endl;
    return false;
  }
  n_bits = (unsigned int)br.read_byte();
  if(n_bits < 9 || n_bits > 20) {
    std::cerr << "Unsupported LZW dictionary size: " << n_bits << " bits" << std::endl;
    return false;
  }
  return true;
}
