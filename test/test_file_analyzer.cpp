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
  std::string s = "abcd";
  std::istringstream iss(s);
  fa->analyze(iss);
  EXPECT_EQ(fa->length(), 4);
}

TEST_F(FileAnalyzerTest, EntropyOfRepeatedSymbolIsZero) {
  std::string s = "aaaa";
  std::istringstream iss(s);
  fa->analyze(iss);
  EXPECT_EQ(fa->entropy(), 0.0);
}

TEST_F(FileAnalyzerTest, EntropyOfTwoEquallyLikelySymbolsIsOne) {
  std::string s = "abab";
  std::istringstream iss(s);
  fa->analyze(iss);
  EXPECT_EQ(fa->entropy(), 1.0);
}

TEST_F(FileAnalyzerTest, EntropyOfFourEquallyLikelySymbolsIsTwo) {
  std::string s = "abcd";
  std::istringstream iss(s);
  fa->analyze(iss);
  EXPECT_EQ(fa->entropy(), 2.0);
}

TEST_F(FileAnalyzerTest, EntropyIsOrderInvariant) {
  std::string s1 = "abab";
  std::istringstream iss1(s1);
  fa->analyze(iss1);
  float ent1 = fa->entropy();

  std::string s2 = "baba";
  std::istringstream iss2(s2);
  fa->analyze(iss2);
  float ent2 = fa->entropy();

  EXPECT_EQ(ent1, ent2);
}
