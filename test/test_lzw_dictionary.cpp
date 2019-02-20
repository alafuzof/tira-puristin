#include "gtest/gtest.h"
#include "lzw_dictionary.h"

class LZWDictionaryTest : public ::testing::Test {
public:
  LZWDictionaryTest() {
    dict = new LZWDictionary(10);
  };
  ~LZWDictionaryTest() {
    delete dict;
  };
  LZWDictionary *dict;
};

TEST_F(LZWDictionaryTest, TestDictIs10Bit) {
  ASSERT_EQ(10, dict->num_bits());
}

TEST_F(LZWDictionaryTest, StartsOutWithAll8BitValues) {
  ASSERT_EQ(256, dict->num_entries());
  for(int c = 0; c<256; c++) {
    ASSERT_NE(-1, dict->query(-1, (unsigned char)c));
  }
}

TEST_F(LZWDictionaryTest, InsertingIncreasesNumberOfEntries) {
  ASSERT_NE(-1, dict->insert(0, 'a'));
  ASSERT_EQ(256+1, dict->num_entries());
}

TEST_F(LZWDictionaryTest, InsertingExistingStringsDoesNotIncreaseNumberOfEntries) {
  ASSERT_NE(-1, dict->insert(-1, 'a')); // No effect
  ASSERT_EQ(256, dict->num_entries());
  ASSERT_NE(-1, dict->insert(1, 'a')); // Adds string to dict
  ASSERT_EQ(256+1, dict->num_entries());
  ASSERT_NE(-1, dict->insert(1, 'a')); // No effect
  ASSERT_EQ(256+1, dict->num_entries());
}

TEST_F(LZWDictionaryTest, ResettingGivesResetCode) {
  ASSERT_EQ((1<<dict->num_bits())-1, dict->reset());
}

TEST_F(LZWDictionaryTest, ResettingDictionaryResetsNumberOfEntries) {
  ASSERT_NE(-1, dict->insert(1, 'a')); // Adds string to dict
  ASSERT_EQ(256+1, dict->num_entries());
  dict->reset();
  ASSERT_EQ(256, dict->num_entries());
}

TEST_F(LZWDictionaryTest, InsertsFailOnceDictionaryIsFull) {
  int prefix = 0;
  for(int i = 256; i < 1023; i++) { // Note 1023 reserved for reset code
    prefix = dict->insert(prefix, 'a');
    ASSERT_NE(-1, prefix);
  }
  prefix = dict->insert(prefix, 'a');
  ASSERT_EQ(-1, prefix);
}
