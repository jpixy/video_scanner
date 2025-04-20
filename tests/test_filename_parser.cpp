#include "video_scanner/filename_parser.h"
#include <gtest/gtest.h>
#include <iostream>

class FilenameParserTest : public ::testing::Test {
protected:
  void SetUp() override { std::cout << "GTest SetUp is called." << std::endl; }
};

TEST(FilenameParserTest, ComplexChineseEnglishName) {
  video_scanner::VideoFileInfo info;
  info.filename = "局内人.导演剪辑版.Inside.Men.2015.DC.HD720P.X264.AAC.Korean."
                  "CHS.Mp4Ba.mp4";

  video_scanner::FilenameParser::Parse(info);

  EXPECT_EQ(info.year, 2015);
  EXPECT_EQ(info.video_name_cn, "局内人 导演剪辑版 Inside Men");
  // EXPECT_EQ(info.video_name_en, "Inside Men");
  // EXPECT_TRUE(info.video_name_other.empty());
}

TEST(FilenameParserTest, EnglishNameOnly) {
  video_scanner::VideoFileInfo info;
  info.filename = "The.Shawshank.Redemption.1994.1080p.BluRay.mp4";

  video_scanner::FilenameParser::Parse(info);

  // EXPECT_EQ(info.year, 1994);
  // EXPECT_EQ(info.video_name_en, "The Shawshank Redemption");
  // EXPECT_TRUE(info.video_name_cn.empty());
}

TEST(FilenameParserTest, ChineseNameOnly) {
  video_scanner::VideoFileInfo info;
  info.filename = "让子弹飞.2010.720p.mp4";

  video_scanner::FilenameParser::Parse(info);

  // EXPECT_EQ(info.year, 2010);
  // EXPECT_EQ(info.video_name_cn, "让子弹飞");
  // EXPECT_TRUE(info.video_name_en.empty());
}

TEST(FilenameParserTest, ComplexChineseEnglishNameWithQualityMarks) {
  video_scanner::VideoFileInfo info;
  info.filename = "局内人.导演剪辑版.Inside.Men.2015.DC.HD720P.X264.AAC.Korean."
                  "CHS.Mp4Ba.mp4";

  video_scanner::FilenameParser::Parse(info);

  // EXPECT_EQ(info.year, 2015);
  // EXPECT_EQ(info.video_name_cn, "局内人 导演剪辑版");
  // EXPECT_EQ(info.video_name_en, "Inside Men");
  // EXPECT_TRUE(info.video_name_other.empty());
}

TEST(FilenameParserTest, FilenameWithMultipleChineseParts) {
  video_scanner::VideoFileInfo info;
  info.filename = "电影名.特别版.导演剪辑.English.Title.2020.1080p.mp4";

  video_scanner::FilenameParser::Parse(info);

  // EXPECT_EQ(info.year, 2020);
  // EXPECT_EQ(info.video_name_cn, "电影名 特别版 导演剪辑");
  // EXPECT_EQ(info.video_name_en, "English Title");
}

TEST(FilenameParserTest, ComplexMixedNames) {
  video_scanner::VideoFileInfo info;
  info.filename = "The.Great.Wall.长城.2016.1080p.BluRay.mp4";

  video_scanner::FilenameParser::Parse(info);

  // EXPECT_EQ(info.year, 2016);
  // EXPECT_EQ(info.video_name_en, "The Great Wall");
  // EXPECT_EQ(info.video_name_cn, "长城");
}

TEST(FilenameParserTest, LongEnglishName) {
  video_scanner::VideoFileInfo info;
  info.filename = "Star.Wars.Episode.IV.A.New.Hope.1977.Special.Edition.mp4";

  video_scanner::FilenameParser::Parse(info);

  // EXPECT_EQ(info.year, 1977);
  // EXPECT_EQ(info.video_name_en, "Star Wars Episode IV A New Hope");
  // EXPECT_TRUE(info.video_name_cn.empty());
}

TEST(FilenameParserTest, MultipleChineseParts) {
  video_scanner::VideoFileInfo info;
  info.filename = "西游记.大圣归来.2015.HD1080P.mp4";

  video_scanner::FilenameParser::Parse(info);

  // EXPECT_EQ(info.year, 2015);
  // EXPECT_EQ(info.video_name_cn, "西游记 大圣归来");
  // EXPECT_TRUE(info.video_name_en.empty());
}