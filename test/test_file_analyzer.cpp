#include <sstream>
#include <string>
#include "gtest/gtest.h"
#include "file_analyzer.h"

class FileAnalyzerTest : public ::testing::Test {
public:
  FileAnalyzerTest() {
    fa = new FileAnalyzer();
  };
  ~FileAnalyzerTest() {
    delete fa;
  };
  FileAnalyzer *fa;
};

TEST_F(FileAnalyzerTest, LengthStartsAsZero) {
  EXPECT_EQ(fa->length(), 0);
}

TEST_F(FileAnalyzerTest, EntropyStartsAsZero) {
  EXPECT_EQ(fa->entropy(), 0.0);
}

TEST_F(FileAnalyzerTest, LengthReflectsAnalyzedData) {
  fa->analyze("abcd");
  EXPECT_EQ(fa->length(), 4);
}

TEST_F(FileAnalyzerTest, EntropyOfRepeatedSymbolIsZero) {
  fa->analyze("aaaa");
  EXPECT_EQ(fa->entropy(), 0.0);
}

TEST_F(FileAnalyzerTest, EntropyOfTwoEquallyLikelySymbolsIsOne) {
  fa->analyze("abab");
  EXPECT_EQ(fa->entropy(), 1.0);
}

TEST_F(FileAnalyzerTest, EntropyOfFourEquallyLikelySymbolsIsTwo) {
  fa->analyze("abcd");
  EXPECT_EQ(fa->entropy(), 2.0);
}

TEST_F(FileAnalyzerTest, EntropyIsOrderInvariant) {
  fa->analyze("abab");
  float ent1 = fa->entropy();

  fa->analyze("baba");
  float ent2 = fa->entropy();

  EXPECT_EQ(ent1, ent2);
}
