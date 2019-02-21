#include "gtest/gtest.h"
#include "bit_writer.h"

class BitWriterTest : public ::testing::Test {
public:
  BitWriterTest() {
    // Stringstream is allocated on heap in order to maintain the data during
    // further function calls
    oss = new std::ostringstream();
    bw = new BitWriter(*oss);
  };
  ~BitWriterTest() {
    delete oss;
    delete bw;
  };
  std::ostringstream *oss;
  BitWriter *bw;
};

TEST_F(BitWriterTest, WriteBitsFromLeftToRight) {
  // 'a' = 0110 0001
  bw->write_bit(false);
  bw->write_bit(true);
  bw->write_bit(true);
  bw->write_bit(false);
  bw->write_bit(false);
  bw->write_bit(false);
  bw->write_bit(false);
  bw->write_bit(true);
  // 'b' = 0110 0001
  bw->write_bit(false);
  bw->write_bit(true);
  bw->write_bit(true);
  bw->write_bit(false);
  bw->write_bit(false);
  bw->write_bit(false);
  bw->write_bit(true);
  bw->write_bit(false);
  // 'c' = 0110 0011
  bw->write_bit(false);
  bw->write_bit(true);
  bw->write_bit(true);
  bw->write_bit(false);
  bw->write_bit(false);
  bw->write_bit(false);
  bw->write_bit(true);
  bw->write_bit(true);
  ASSERT_STREQ("abc", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteBytesFromLeftToRight) {
  bw->write_byte('a');
  bw->write_byte('b');
  bw->write_byte('c');
  ASSERT_STREQ("abc", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteNonAlignedByte) {
  bw->write_bit(false); // (0xxxxxxx)
  bw->write_byte(194);  // 01100001 (0xxxxxxx)
  bw->write_byte(196);  // 01100001 01100010 (0xxxxxxx)
  bw->write_bit(true);  // 01100001 01100010 (01xxxxxx)
  bw->write_bit(true);  // 01100001 01100010 (011xxxxx)
  bw->write_bit(false); // 01100001 01100010 (0110xxxx)
  bw->write_bit(false); // 01100001 01100010 (01100xxx)
  bw->write_bit(false); // 01100001 01100010 (011000xx)
  bw->write_bit(true);  // 01100001 01100010 (0110001x)
  bw->write_bit(true);  // 01100001 01100010 01100011
  ASSERT_STREQ("abc", oss->str().c_str());
}

TEST_F(BitWriterTest, FlushWriteBufferToStream) {
  bw->write_bit(false); // (0xxxxxxx)
  bw->write_bit(true);  // (01xxxxxx)
  bw->flush();          // 01000000 = '@'
  ASSERT_STREQ("@", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteBitUsingOperator) {
  *bw << false; // (0xxxxxxx)
  *bw << true;  // (01xxxxxx)
  bw->flush();  // 01000000 = '@'
  ASSERT_STREQ("@", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteByteUsingOperator) {
  *bw << 'A';
  ASSERT_STREQ("A", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteString) {
  bw->write_string("abc");
  ASSERT_STREQ("abc", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteInt) {
  bw->write_int(1633837824);
  ASSERT_STREQ("abc", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteBitString) {
  bw->write_bitstring("0110 0001 0000 0000");
  ASSERT_STREQ("a", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteUint4) {
  bw->write_uint(6, 4);
  bw->write_uint(1, 4);
  bw->write_uint(6, 4);
  bw->write_uint(2, 4);
  bw->write_uint(6, 4);
  bw->write_uint(3, 4);
  bw->write_byte('\0');
  ASSERT_STREQ("abc", oss->str().c_str());
}

TEST_F(BitWriterTest, WriteUint6) {
  bw->write_uint(24, 6);
  bw->write_uint(22, 6);
  bw->write_uint(9, 6);
  bw->write_uint(35, 6);
  bw->write_byte('\0');
  ASSERT_STREQ("abc", oss->str().c_str());
}
