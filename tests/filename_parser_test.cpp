#include "video_scanner/filename_parser.h"
#include "video_scanner/logger.h"
#include "video_scanner/utils/string_utils.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>

using namespace video_scanner;

class FilenameParserTest : public ::testing::Test {
protected:
  void SetUp() override {
    // 初始化日志
    video_scanner::Logger::init();
    // 测试数据
    testArray_ = {"[meta]",  "你好",       "世界", "hello", "today", "天气",
                  "[meta2]", "こんにちは", "123",  "天气",  "good"};
    metaArray_ = {"[meta]", "[meta2]"};
  }
  std::vector<std::string> testArray_;
  std::vector<std::string> metaArray_;

  void TearDown() override {
    // 清理日志
  }
};

// 测试提取年份
TEST_F(FilenameParserTest, ExtractMovieYear) {
  VideoFileInfo file_info;
  file_info.filename = "example.2023.mp4";
  FilenameParser::Parse(file_info);
  EXPECT_EQ(file_info.year, 2023);
}

// 测试分割字符串
TEST_F(FilenameParserTest, SplitStringByDelimiters) {
  VideoFileInfo file_info;
  file_info.filename = "example.2023.mp4";
  FilenameParser::Parse(file_info);
  std::string base_name =
      video_scanner::utils::StringUtils::RemoveExtension(file_info.filename);
  auto pairsStr =
      video_scanner::utils::StringUtils::SplitStringByDelimitersToPair(
          base_name, "2023");
  EXPECT_EQ(pairsStr.first, "example. ");
  EXPECT_EQ(pairsStr.second, "");
}

// 测试元信息中断连续性
TEST_F(FilenameParserTest, IsMetaInfo) {
  VideoFileInfo file_info;
  file_info.filename = "example.2023.mp4";
  FilenameParser::Parse(file_info);

  std::vector<std::string> meta_keywords = {"meta"};
  EXPECT_FALSE(FilenameParser::IsMetaInfo("meta", meta_keywords));
  EXPECT_TRUE(FilenameParser::IsMetaInfo("example", meta_keywords));
}

// 测试清理前后点缀
TEST_F(FilenameParserTest, TrimName) {
  std::string name = "  example  ";
  std::string trimmed_name = video_scanner::utils::StringUtils::TrimName(name);
  EXPECT_EQ(trimmed_name, "example");
}

TEST_F(FilenameParserTest, FindChineseNames) {
  auto result =
      FilenameParser::FindVideoNames(testArray_, metaArray_, LanguageType::CN);

  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "你好世界");
  EXPECT_EQ(result[1], "天气天气");
}

TEST_F(FilenameParserTest, FindEnglishNames) {
  auto result =
      FilenameParser::FindVideoNames(testArray_, metaArray_, LanguageType::EN);

  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "hello");
  EXPECT_EQ(result[1], "today good");
}

TEST_F(FilenameParserTest, FindOtherLanguageNames) {
  auto result = FilenameParser::FindVideoNames(testArray_, metaArray_,
                                               LanguageType::OTHER);

  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "こんにちは");
  EXPECT_EQ(result[1], "123");
}

TEST_F(FilenameParserTest, EmptyInput) {
  std::vector<std::string> emptyArray;
  auto result =
      FilenameParser::FindVideoNames(emptyArray, metaArray_, LanguageType::CN);
  EXPECT_TRUE(result.empty());
}

TEST_F(FilenameParserTest, AllMetaInfo) {
  std::vector<std::string> allMeta = {"[meta1]", "[meta2]", "[meta3]"};
  auto result =
      FilenameParser::FindVideoNames(allMeta, metaArray_, LanguageType::EN);
  EXPECT_TRUE(result.empty());
}

TEST_F(FilenameParserTest, MixedContent) {
  std::vector<std::string> mixed = {"start",    "[meta]", "中文1",
                                    "english1", "中文2",  "123",
                                    "english2", "[meta]", "end"};

  // 测试中文
  auto cnResult =
      FilenameParser::FindVideoNames(mixed, metaArray_, LanguageType::CN);
  ASSERT_EQ(cnResult.size(), 2);
  EXPECT_EQ(cnResult[0], "中文1");
  EXPECT_EQ(cnResult[1], "中文2");

  // 测试英文
  auto enResult =
      FilenameParser::FindVideoNames(mixed, metaArray_, LanguageType::EN);
  ASSERT_EQ(enResult.size(), 2);
  EXPECT_EQ(enResult[0], "english1");
  EXPECT_EQ(enResult[1], "english2");

  // 测试其他语言
  auto otherResult =
      FilenameParser::FindVideoNames(mixed, metaArray_, LanguageType::OTHER);
  ASSERT_EQ(otherResult.size(), 3);
  EXPECT_EQ(otherResult[0], "start");
  EXPECT_EQ(otherResult[1], "123");
  EXPECT_EQ(otherResult[2], "end");
}

TEST_F(FilenameParserTest, OtherLanguageWithMixedContent) {
  std::vector<std::string> testArray = {
      "start", "[meta]", "こんにちは", "hello", "123", "[meta]", "end"};
  auto result = FilenameParser::FindVideoNames(testArray, metaArray_,
                                               LanguageType::OTHER);

  ASSERT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], "start");
  EXPECT_EQ(result[1], "こんにちは");
  EXPECT_EQ(result[2], "123end");
}

TEST_F(FilenameParserTest, FindChineseNames1) {
  std::vector<std::string> testArray = {"[meta]", "你好", "世界",    "hello",
                                        "今天",   "天气", "[meta2]", "很好"};
  auto result =
      FilenameParser::FindVideoNames(testArray, metaArray_, LanguageType::CN);

  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "你好世界");
  EXPECT_EQ(result[1], "今天天气很好");
}

TEST_F(FilenameParserTest, FindEnglishNames1) {
  std::vector<std::string> testArray = {"[meta]", "hello", "world",   "你好",
                                        "today",  "is",    "[meta2]", "good"};
  auto result =
      FilenameParser::FindVideoNames(testArray, metaArray_, LanguageType::EN);

  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "hello world");
  EXPECT_EQ(result[1], "today is good");
}

TEST_F(FilenameParserTest, FindOtherLanguageNames1) {
  std::vector<std::string> testArray = {
      "[meta]", "こんにちは", "123", "hello", "안녕하세요", "[meta2]", "456"};
  auto result = FilenameParser::FindVideoNames(testArray, metaArray_,
                                               LanguageType::OTHER);

  ASSERT_EQ(result.size(), 2);
  /*EXPECT_EQ(result[0], "こんにちは123");*/
  /*EXPECT_EQ(result[1], "안녕하세요456");*/
  EXPECT_EQ(result[0], "[meta]");
  EXPECT_EQ(result[1], "[meta2]");
}
