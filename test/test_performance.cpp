#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "gtest/gtest.h"
#include "huffman_code.h"

class DickensTest : public ::testing::Test {
public:
  DickensTest() {
    hc = new HuffmanCode();
    std::ifstream t("./test_data/dickens");
    str = new std::string((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
  };
  ~DickensTest() {
    delete hc;
  };
  HuffmanCode *hc;
  std::string *str;
};

TEST_F(DickensTest, DickensHuffman) {
  unsigned int sizes[] = {1,2,3,4,5,6,7,8,9,10};
  std::cout << "Huffman coding (Dickens)" << std::endl;
  for(int i=0; i<10; i++) {
    std::string substr = str->substr(0, sizes[i]*1024*1024);
    std::istringstream iss(substr);
    std::stringstream ss;
    std::ostringstream oss;
    clock_t t1 = clock();
    hc->encode(iss, ss);
    clock_t t2 = clock();
    hc->decode(ss, oss);
    clock_t t3 = clock();
    std::cout << std::setw(2) << sizes[i] << " MB "
              << "encoding: " << (((float)(t2-t1))/CLOCKS_PER_SEC)*1000 << " ms "
              << "decoding: " << (((float)(t3-t2))/CLOCKS_PER_SEC)*1000 << " ms "
              << "compression ratio: " << ((float)iss.str().length())/ss.str().length()
              << std::endl;
    ASSERT_STREQ(iss.str().c_str(), oss.str().c_str());
  }
}
