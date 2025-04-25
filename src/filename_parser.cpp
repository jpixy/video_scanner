#include "video_scanner/filename_parser.h"
#include "video_scanner/logger.h"
#include <algorithm>
#include <cctype>
#include <regex>
#include <string>
#include <video_scanner/utils/string_utils.h>

namespace video_scanner {

void FilenameParser::Parse(VideoFileInfo &file_info) {
  using video_scanner::utils::StringUtils;

  video_scanner::Logger::init();
  LOG_INFO("Starting to parse filename: {}", file_info.filename);

  file_info.year = 0;
  file_info.video_name_cn.clear();
  file_info.video_name_en.clear();
  file_info.video_name_other.clear();

  // 1. 去除后缀名
  std::string base_name =
      video_scanner::utils::StringUtils::RemoveExtension(file_info.filename);
  LOG_INFO("Base name after removing quality info: {}", base_name);

  // 2. 查找文件名中是否有年份
  bool existYear = StringUtils::ExtractMovieYear(base_name, file_info.year);
  LOG_INFO("Extracted year: {}", file_info.year);

  // 3. 如果有年份，以年份为分割点
  if (existYear) {
    auto pairsStr = StringUtils::SplitStringByDelimitersToPair(
        base_name, std::to_string(file_info.year));

    // NOTE : 需要注意这里的分隔符集合
    const std::string delimiters = ".,_- ";
    std::vector<std::string> strArray =
        StringUtils::SplitStringByDelimitersToArray(pairsStr.first, delimiters);
  }

  static const std::vector<std::string> meta_keywords = {
      "DC",     "HD",   "720P",   "1080P",    "4K",     "X264",   "AAC",
      "Korean", "CHS",  "CHT",    "BluRay",   "WEB-DL", "HDR",    "REMUX",
      "DTS",    "AC3",  "H265",   "HEVC",     "AAC5.1", "DDP5.1", "Atmos",
      "SDR",    "DV",   "HDR10",  "HDR10+",   "IMAX",   "4K",     "60帧",
      "fps",    "sub",  "KORSUB", "WEBRip",   "H264",   "H265",   "2Audio",
      "多国",   "国语", "中字",   "中简繁英", "字幕"};

  // 清理前后点缀
  file_info.video_name_cn = StringUtils::TrimName(file_info.video_name_cn);
  file_info.video_name_en = StringUtils::TrimName(file_info.video_name_en);
  file_info.video_name_other =
      StringUtils::TrimName(file_info.video_name_other);
}

bool FilenameParser::IsMetaInfo(const std::string &str,
                                const std::vector<std::string> &metaArray) {
  for (const auto &substr : metaArray) {
    if (str.find(substr) != std::string::npos) {
      return false;
    }
  }
  return true;
}

std::vector<std::string>
FilenameParser::FindVideoNames(const std::vector<std::string> &strArray,
                               const std::vector<std::string> &metaArray,
                               LanguageType type) {
  using video_scanner::utils::StringUtils;

  std::vector<std::string> result;
  std::string current_sequence;

  auto isTargetLanguage = [&](const std::string &str) {
    if (IsMetaInfo(str, metaArray))
      return false;

    switch (type) {
    case LanguageType::CN:
      return StringUtils::isChinese(str) && !StringUtils::isEnglish(str);
    case LanguageType::EN:
      return !StringUtils::isChinese(str) && StringUtils::isEnglish(str);
    default: // Other
      return !StringUtils::isChinese(str) && !StringUtils::isEnglish(str);
    }
  };

  for (const auto &str : strArray) {
    if (isTargetLanguage(str)) {
      if (type == LanguageType::EN && !current_sequence.empty()) {
        current_sequence += " " + str;
      } else {
        current_sequence += str;
      }
    } else {
      if (!current_sequence.empty()) {
        result.push_back(current_sequence);
        current_sequence.clear();
      }
    }
  }

  if (!current_sequence.empty()) {
    result.push_back(current_sequence);
  }

  return result;
}

} // namespace video_scanner
