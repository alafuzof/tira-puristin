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

TEST_F(DickensTest, Dickens1kBHuffman) {
  std::string substr = str->substr(0, 1024);
  std::istringstream iss(substr);
  std::ostringstream oss;
  clock_t start = clock();
  hc->encode(iss, oss);
  clock_t stop = clock();
  std::cout << "1 kB Dickens " <<  (((float)(stop-start))/CLOCKS_PER_SEC)*1000 << " ms" << std::endl;
  ASSERT_GT(stop-start, 0);
}

TEST_F(DickensTest, Dickens10kBHuffman) {
  std::string substr = str->substr(0, 1024*10);
  std::istringstream iss(substr);
  std::ostringstream oss;
  clock_t start = clock();
  hc->encode(iss, oss);
  clock_t stop = clock();
  std::cout << "10 kB Dickens " <<  (((float)(stop-start))/CLOCKS_PER_SEC)*1000 << " ms" << std::endl;
  ASSERT_GT(stop-start, 0);
}

TEST_F(DickensTest, Dickens100kBHuffman) {
  std::string substr = str->substr(0, 1024*100);
  std::istringstream iss(substr);
  std::ostringstream oss;
  clock_t start = clock();
  hc->encode(iss, oss);
  clock_t stop = clock();
  std::cout << "100 kB Dickens " <<  (((float)(stop-start))/CLOCKS_PER_SEC)*1000 << " ms" << std::endl;
  ASSERT_GT(stop-start, 0);
}

TEST_F(DickensTest, Dickens1MBHuffman) {
  std::string substr = str->substr(0, 1024*1024);
  std::istringstream iss(substr);
  std::ostringstream oss;
  clock_t start = clock();
  hc->encode(iss, oss);
  clock_t stop = clock();
  std::cout << "1 MB Dickens " <<  (((float)(stop-start))/CLOCKS_PER_SEC)*1000 << " ms" << std::endl;
  ASSERT_GT(stop-start, 0);
}

TEST_F(DickensTest, Dickens10MBHuffman) {
  std::string substr = str->substr(0, 1024*1024*10);
  std::istringstream iss(substr);
  std::ostringstream oss;
  clock_t start = clock();
  hc->encode(iss, oss);
  clock_t stop = clock();
  std::cout << "10 MB Dickens " <<  (((float)(stop-start))/CLOCKS_PER_SEC)*1000 << " ms" << std::endl;
  ASSERT_GT(stop-start, 0);
}
