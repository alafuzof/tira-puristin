#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "file_analyzer.h"
#include "huffman_code.h"
#include "lzw_code.h"

class PerformanceTest : public ::testing::Test {
public:
  PerformanceTest() {
    fa = new FileAnalyzer();
    hc = new HuffmanCode();
    lzw = new LZWCode();

    std::ifstream dickens_if("./test_data/dickens");
    dickens_str = new std::string((std::istreambuf_iterator<char>(dickens_if)),
                                   std::istreambuf_iterator<char>());

    std::ifstream xml_if("./test_data/xml");
    xml_str = new std::string((std::istreambuf_iterator<char>(xml_if)),
                                  std::istreambuf_iterator<char>());
  };
  ~PerformanceTest() {
    delete fa;
    delete hc;
    delete lzw;
    delete dickens_str;
    delete xml_str;
  };

  FileAnalyzer *fa;
  HuffmanCode *hc;
  LZWCode *lzw;
  std::string *dickens_str;
  std::string *xml_str;
};

TEST_F(PerformanceTest, DickensFileAnalyzer) {
  unsigned int sizes[] = {1,2,3,4,5,6,7,8,9,10};
  std::cout << "FileAnalyzer (Dickens)" << std::endl;
  for(int i=0; i<10; i++) {
    std::string substr = dickens_str->substr(0, sizes[i]*1024*1024);
    clock_t t1 = clock();
    fa->analyze(substr);
    clock_t t2 = clock();
    std::cout << "  " << std::setw(2) << sizes[i] << " MB "
              << "analysis: " << (((float)(t2-t1))/CLOCKS_PER_SEC)*1000 << " ms "
              << std::endl;
    ASSERT_GE(sizes[i]*1024*1024, fa->length()) << i; // GE because the Dickens file is under 10 MB
  }
}

TEST_F(PerformanceTest, DickensHuffman) {
  unsigned int sizes[] = {1,2,3,4,5,6,7,8,9,10};
  std::cout << "Huffman coding (Dickens)" << std::endl;
  for(int i=0; i<10; i++) {
    std::string substr = dickens_str->substr(0, sizes[i]*1024*1024);
    std::istringstream iss(substr);
    std::stringstream ss;
    std::ostringstream oss;
    clock_t t1 = clock();
    hc->encode(iss, ss);
    clock_t t2 = clock();
    hc->decode(ss, oss);
    clock_t t3 = clock();
    std::cout << "  " << std::setw(2) << sizes[i] << " MB "
              << "encoding: " << (((float)(t2-t1))/CLOCKS_PER_SEC)*1000 << " ms "
              << "decoding: " << (((float)(t3-t2))/CLOCKS_PER_SEC)*1000 << " ms "
              << "compression ratio: " << ((float)iss.str().length())/ss.str().length()
              << std::endl;
    ASSERT_STREQ(iss.str().c_str(), oss.str().c_str());
  }
}

TEST_F(PerformanceTest, DickensLZW) {
  unsigned int sizes[] = {1,2,3,4,5,6,7,8,9,10};
  std::cout << "LZW coding (Dickens)" << std::endl;
  for(int n_bits=9; n_bits<21; n_bits++) {
    std::cout << "  " << n_bits << " bits:" << std::endl;
    for(int i=0; i<10; i++) {
      std::string substr = dickens_str->substr(0, sizes[i]*1024*1024);
      std::istringstream iss(substr);
      std::stringstream ss;
      std::ostringstream oss;
      clock_t t1 = clock();
      lzw->encode(iss, ss, n_bits);
      clock_t t2 = clock();
      lzw->decode(ss, oss);
      clock_t t3 = clock();
      std::cout << "    " << std::setw(2) << sizes[i] << " MB "
                << "encoding: " << (((float)(t2-t1))/CLOCKS_PER_SEC)*1000 << " ms "
                << "decoding: " << (((float)(t3-t2))/CLOCKS_PER_SEC)*1000 << " ms "
                << "compression ratio: " << ((float)iss.str().length())/ss.str().length()
                << std::endl;
      ASSERT_STREQ(iss.str().c_str(), oss.str().c_str());
    }
  }
}
