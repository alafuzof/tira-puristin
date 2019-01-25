#include "gtest/gtest.h"
#include "file_analyzer.h"

TEST(FileAnalyzer, LengthStartsAsZero) {
  FileAnalyzer fa;
  EXPECT_EQ(fa.length(), 0);
}
