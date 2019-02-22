#include "gtest/gtest.h"
#include "lzw_code.h"

class LZWCodeTest : public ::testing::Test {
public:
  LZWCodeTest() {
    lzw = new LZWCode;
    iss = new std::istringstream("Lorem ipsum dolor sit amet, consectetur "
      "adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore "
      "magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
      "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute "
      "irure dolor in reprehenderit in voluptate velit esse cillum dolore eu "
      "fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, "
      "sunt in culpa qui officia deserunt mollit anim id est laborum.",
      std::ios_base::in|std::ios_base::binary);
    oss = new std::ostringstream();
    ss = new std::stringstream();
  }
  ~LZWCodeTest() {
    delete lzw;
    delete iss;
    delete oss;
    delete ss;
  }
  LZWCode *lzw;
  std::istringstream *iss;
  std::ostringstream *oss;
  std::stringstream *ss;
};

TEST_F(LZWCodeTest, CompressedDataCanBeRestored) {
  lzw->encode(*iss, *ss, 10);
  lzw->decode(*ss, *oss);
  EXPECT_STREQ(iss->str().c_str(), oss->str().c_str());
}
