#include "gtest/gtest.h"
#include "lzw_code.h"

class LZWCodeTest : public ::testing::Test {
public:
  LZWCodeTest() {
    lzw = new LZWCode();
    iss = nullptr; // Initialized in the unit test
    oss = new std::ostringstream();
    ss = new std::stringstream();
  }
  ~LZWCodeTest() {
    if(iss != nullptr)
      delete iss;
    if(oss != nullptr)
      delete oss;
    if(ss != nullptr)
      delete ss;
    delete lzw;
  }
  LZWCode *lzw;
  std::istringstream *iss;
  std::ostringstream *oss;
  std::stringstream *ss;
};

TEST_F(LZWCodeTest, CompressedDataCanBeRestored) {
  for(int i=9; i<21; i++) {
    iss = new std::istringstream("Lorem ipsum dolor sit amet, consectetur "
      "adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore "
      "magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
      "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute "
      "irure dolor in reprehenderit in voluptate velit esse cillum dolore eu "
      "fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, "
      "sunt in culpa qui officia deserunt mollit anim id est laborum.",
      std::ios_base::in|std::ios_base::binary);
    //oss = new std::ostringstream();
    //ss = new std::stringstream();

    lzw->encode(*iss, *ss, i);
    ASSERT_EQ(0, lzw->decode(*ss, *oss)) << i;
    EXPECT_STREQ(iss->str().c_str(), oss->str().c_str()) << i;

    delete iss; iss = nullptr;
    ss->str(""); ss->clear();
    oss->str(""); oss->clear();
  }
}

TEST_F(LZWCodeTest, CompressedDataCanBeRestoredWithNewlines) {
  for(int i=9; i<21; i++) {
    iss = new std::istringstream("**The Project Gutenberg Etext of A Child's "
    "History of England**\r\n#11 in our series by Charles Dickens\r\n\r\n\r\n",
      std::ios_base::in|std::ios_base::binary);
    //oss = new std::ostringstream();
    //ss = new std::stringstream();

    lzw->encode(*iss, *ss, i);
    ASSERT_EQ(0, lzw->decode(*ss, *oss)) << i;
    EXPECT_STREQ(iss->str().c_str(), oss->str().c_str()) << i;

    delete iss; iss = nullptr;
    ss->str(""); ss->clear();
    oss->str(""); oss->clear();
  }
}
