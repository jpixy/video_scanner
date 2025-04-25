#include "video_scanner/utils/string_utils.h"
#include <gtest/gtest.h>
#include <iostream>

namespace video_scanner {
namespace utils {
namespace test {

void printVector(const std::vector<std::string> &vec);

class StringUtilsTest : public ::testing::Test {
protected:
  void SetUp() override {
    // 测试前的初始化代码（如有需要）
  }

  void TearDown() override {
    // 测试后的清理代码（如有需要）
  }

  // 测试数据
  const std::string filename_with_ext = "document.pdf";
  const std::string filename_without_ext = "report";
  const std::string filename_multiple_dots = "archive.tar.gz";
};

void printVector(const std::vector<std::string> &vec) {
  for (const auto &str : vec) {
    std::cout << str << " ";
  }
  std::cout << std::endl;
}

TEST_F(StringUtilsTest, RemoveExtension) {
  // 测试移除文件扩展名
  EXPECT_EQ(StringUtils::RemoveExtension(filename_with_ext), "document");
  EXPECT_EQ(StringUtils::RemoveExtension(filename_without_ext), "report");
  EXPECT_EQ(StringUtils::RemoveExtension(filename_multiple_dots),
            "archive.tar");
  EXPECT_EQ(StringUtils::RemoveExtension(""), "");
  EXPECT_EQ(StringUtils::RemoveExtension(".hiddenfile"), "");
}

TEST_F(StringUtilsTest, ExtractMovieYear) {
  int year = 0;

  // 测试从文件名中提取年份
  EXPECT_TRUE(StringUtils::ExtractMovieYear("report_2023.pdf", year));
  EXPECT_EQ(year, 2023);

  EXPECT_TRUE(StringUtils::ExtractMovieYear("data_1999_v2.csv", year));
  EXPECT_EQ(year, 1999);

  EXPECT_TRUE(StringUtils::ExtractMovieYear("data_1809_v2.csv", year));
  EXPECT_EQ(year, 1809);

  EXPECT_FALSE(StringUtils::ExtractMovieYear("data_1709_v2.csv", year));

  EXPECT_TRUE(StringUtils::ExtractMovieYear("2021年度总结.docx", year));
  EXPECT_EQ(year, 2021);

  // 测试失败情况
  EXPECT_FALSE(StringUtils::ExtractMovieYear("nodatehere.txt", year));
  EXPECT_FALSE(StringUtils::ExtractMovieYear("", year));
  EXPECT_FALSE(StringUtils::ExtractMovieYear("invalid_year_20xx.log", year));
}

TEST_F(StringUtilsTest, SplitStringByDelimitersToPair) {
  // 测试按分隔符分割字符串为pair
  auto result1 = StringUtils::SplitStringByDelimitersToPair("key:value", ":");
  EXPECT_EQ(result1.first, "key");
  EXPECT_EQ(result1.second, "value");

  auto result2 =
      StringUtils::SplitStringByDelimitersToPair("first,second,third", ",");
  EXPECT_EQ(result2.first, "first");
  EXPECT_EQ(result2.second, "second,third");

  // 测试不包含分隔符的情况
  auto result3 = StringUtils::SplitStringByDelimitersToPair("nodelimiter", ",");
  EXPECT_EQ(result3.first, "nodelimiter");
  EXPECT_EQ(result3.second, "");

  // 测试空字符串
  auto result4 = StringUtils::SplitStringByDelimitersToPair("", ":");
  EXPECT_EQ(result4.first, "");
  EXPECT_EQ(result4.second, "");

  auto result5 = StringUtils::SplitStringByDelimitersToPair(
      "夜孔雀.Night.Peacock.2016.HD2160P.X264.AAC.Mandarin.CHS-ENG.Mp4Ba.mp4",
      "2016");
  EXPECT_EQ(result5.first, "夜孔雀.Night.Peacock.");
  EXPECT_EQ(result5.second, ".HD2160P.X264.AAC.Mandarin.CHS-ENG.Mp4Ba.mp4");
}

TEST_F(StringUtilsTest, SplitStringByDelimitersToArray) {
  // 测试按分隔符分割字符串为数组
  auto result1 = StringUtils::SplitStringByDelimitersToArray("a,b,c,d", ",");
  ASSERT_EQ(result1.size(), 4);
  EXPECT_EQ(result1[0], "a");
  EXPECT_EQ(result1[1], "b");
  EXPECT_EQ(result1[2], "c");
  EXPECT_EQ(result1[3], "d");

  // 测试多字符分隔符
  auto result2 =
      StringUtils::SplitStringByDelimitersToArray("one::two::three", "::");
  ASSERT_EQ(result2.size(), 3);
  EXPECT_EQ(result2[0], "one");
  EXPECT_EQ(result2[1], "two");
  EXPECT_EQ(result2[2], "three");

  // 测试不包含分隔符的情况
  auto result3 = StringUtils::SplitStringByDelimitersToArray("singleword", ",");
  ASSERT_EQ(result3.size(), 1);
  EXPECT_EQ(result3[0], "singleword");

  // 测试空字符串
  auto result4 = StringUtils::SplitStringByDelimitersToArray("", ":");
  EXPECT_TRUE(result4.empty());

  // 测试只有分隔符的情况
  auto result5 = StringUtils::SplitStringByDelimitersToArray(",,,", ",");
  printVector(result5);
  ASSERT_EQ(result5.size(), 0);

  auto result6 = StringUtils::SplitStringByDelimitersToArray(
      "夜孔雀.Night_Peacock.2016.HD2160P.X264 AAC.Mandarin.CHS-ENG.Mp4Ba",
      ".,_- ");
  printVector(result6);
  ASSERT_EQ(result6.size(), 11);
  EXPECT_EQ(result6[0], "夜孔雀");
  EXPECT_EQ(result6[1], "Night");
  EXPECT_EQ(result6[2], "Peacock");
}

// 测试 isEnglishWord 功能
TEST_F(StringUtilsTest, IsEnglish) {
  EXPECT_TRUE(StringUtils::isEnglish("hello"));
  EXPECT_TRUE(StringUtils::isEnglish("don't"));
  EXPECT_TRUE(StringUtils::isEnglish("state-of-the-art"));

  EXPECT_FALSE(StringUtils::isEnglish(""));
  EXPECT_FALSE(StringUtils::isEnglish("123"));
  EXPECT_FALSE(StringUtils::isEnglish("hello!"));
  EXPECT_FALSE(StringUtils::isEnglish("'hello"));
}

// 测试 isChinese 功能
TEST_F(StringUtilsTest, IsChinese) {
  EXPECT_TRUE(StringUtils::isChinese("你好"));
  EXPECT_FALSE(StringUtils::isChinese("N_你好"));
  EXPECT_FALSE(StringUtils::isChinese("，。！"));
  EXPECT_FALSE(StringUtils::isChinese("蝉时雨 蝉しぐれ "));
  EXPECT_FALSE(StringUtils::isChinese("蝉时雨 蝉しぐれ 2005"));

  EXPECT_FALSE(StringUtils::isChinese(""));
  EXPECT_FALSE(StringUtils::isChinese("hello"));
  EXPECT_FALSE(StringUtils::isChinese("123"));
}

TEST_F(StringUtilsTest, TrimName) {
  EXPECT_EQ(StringUtils::TrimName("hello_world"), "hello world");
  EXPECT_EQ(StringUtils::TrimName("__hello___world__"), "hello world");
  EXPECT_EQ(StringUtils::TrimName("  hello  world  "), "hello world");
  EXPECT_EQ(StringUtils::TrimName("\thello\tworld\t"), "hello\tworld");
  EXPECT_EQ(StringUtils::TrimName(""), "");
  EXPECT_EQ(StringUtils::TrimName("___"), "");
  EXPECT_EQ(StringUtils::TrimName("   "), "");
}

} // namespace test
} // namespace utils
} // namespace video_scanner
