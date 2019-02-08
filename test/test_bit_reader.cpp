#include "gtest/gtest.h"
#include "bit_reader.h"

class BitReaderTest : public ::testing::Test {
public:
  BitReaderTest() {
    // Stringstream is allocated on heap in order to maintain the data during
    // further function calls
    std::string str = "abcd";
    str[3] = '\0';
    iss = new std::istringstream(str, std::ios_base::in|std::ios_base::binary);
    br = new BitReader(*iss);
  };
  ~BitReaderTest() {
    delete iss;
    delete br;
  };

  std::istringstream *iss;
  BitReader *br;
};

TEST_F(BitReaderTest, ReadBitsFromLeftToRight) {
  // 'a' = 0110 0001
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  // 'b' = 0110 0010
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  // 'c' = 0110 0011
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
  ASSERT_EQ(true, br->read_bit());
}

TEST_F(BitReaderTest, ReadBytesFromLeftToRight) {
  ASSERT_EQ('a', br->read_byte());
  ASSERT_EQ('b', br->read_byte());
  ASSERT_EQ('c', br->read_byte());
}

TEST_F(BitReaderTest, ReadNonAlignedByte) {
  // The first two bytes are 01100001 01100010
  // Try reading 0(1100001 0)110 0010 --> 128+64+2=194
  ASSERT_EQ(false, br->read_bit());
  ASSERT_EQ(194, br->read_byte());
}

TEST_F(BitReaderTest, ReadBitUsingOperator) {
  bool bit = true;
  *br >> bit;
  EXPECT_EQ(false, bit);
}

TEST_F(BitReaderTest, ReadByteUsingOperator) {
  unsigned char byte = '\0';
  *br >> byte;
  EXPECT_EQ('a', byte);
}

TEST_F(BitReaderTest, ReadString) {
  EXPECT_STREQ("abc", br->read_string().c_str());
}

TEST_F(BitReaderTest, ReadInt) {
  // "abc\0" has the below value, checked using online converter
  EXPECT_EQ(1633837824, br->read_int());
}
