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
  EXPECT_EQ(0, fa->length());
}

TEST_F(FileAnalyzerTest, EntropyStartsAsZero) {
  EXPECT_EQ(0.0, fa->entropy());
}

TEST_F(FileAnalyzerTest, LengthReflectsAnalyzedData) {
  fa->analyze("abcd");
  EXPECT_EQ(4, fa->length());
}

TEST_F(FileAnalyzerTest, EntropyOfRepeatedSymbolIsZero) {
  fa->analyze("aaaa");
  EXPECT_EQ(0.0, fa->entropy());
}

TEST_F(FileAnalyzerTest, EntropyOfTwoEquallyLikelySymbolsIsOne) {
  fa->analyze("abab");
  EXPECT_EQ(1.0, fa->entropy());
}

TEST_F(FileAnalyzerTest, EntropyOfFourEquallyLikelySymbolsIsTwo) {
  fa->analyze("abcd");
  EXPECT_EQ(2.0, fa->entropy());
}

TEST_F(FileAnalyzerTest, EntropyIsOrderInvariant) {
  fa->analyze("abab");
  float ent1 = fa->entropy();

  fa->analyze("baba");
  float ent2 = fa->entropy();

  EXPECT_EQ(ent1, ent2);
}

TEST_F(FileAnalyzerTest, FileAnalyzerRestoresStreamPosition) {
  std::istringstream input("abcd");
  std::streampos start_pos = input.tellg();
  fa->analyze(input);
  std::streampos final_pos = input.tellg();

  EXPECT_EQ(start_pos, final_pos);
}

TEST_F(FileAnalyzerTest, FrequenciesAreZeroBeforeAnalysis) {
  unsigned int *freq = fa->frequencies();
  for(unsigned int i=0; i<256; i++) {
    EXPECT_EQ(0, freq[i]);
  }
}

TEST_F(FileAnalyzerTest, FrequenciesReflectAnalyzedInput) {
  fa->analyze("abbcccdddd");
  unsigned int *freq = fa->frequencies();
  EXPECT_EQ(1, freq['a']);
  EXPECT_EQ(2, freq['b']);
  EXPECT_EQ(3, freq['c']);
  EXPECT_EQ(4, freq['d']);
}

TEST_F(FileAnalyzerTest, PrintingReportDoesNotThrowException) {
  std::ostringstream oss;
  EXPECT_NO_THROW(fa->print_report(oss));
}
