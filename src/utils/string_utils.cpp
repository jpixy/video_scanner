#include "video_scanner/utils/string_utils.h"
#include <algorithm>
#include <cctype>
#include <clocale>
#include <cstdint>
#include <regex>
#include <string>
#include <vector>

namespace video_scanner {
namespace utils {

std::string StringUtils::RemoveExtension(const std::string &filename) {
  size_t last_dot = filename.find_last_of('.');
  return (last_dot != std::string::npos) ? filename.substr(0, last_dot)
                                         : filename;
}

bool StringUtils::ExtractMovieYear(const std::string &filename, int &year) {
  std::regex year_regex(R"((18|19|20)\d{2})");
  std::smatch match;

  if (std::regex_search(filename, match, year_regex)) {
    year = std::stoi(match.str());
    return true;
  }
  return false;
}

std::pair<std::string, std::string>
StringUtils::SplitStringByDelimitersToPair(const std::string &str,
                                           const std::string &delimiters) {

  size_t pos = str.find(delimiters);
  if (pos == std::string::npos) {
    return {str, ""};
  }
  return {str.substr(0, pos), str.substr(pos + delimiters.length())};
}

std::vector<std::string>
StringUtils::SplitStringByDelimitersToArray(const std::string &str,
                                            const std::string &delimiters) {

  std::vector<std::string> parts;
  if (str.empty())
    return parts;

  /*const std::string delimiters = "._- ";*/

  size_t start = 0;
  size_t end = str.find_first_of(delimiters);

  while (end != std::string::npos) {
    // 只有当非空时才添加
    if (end != start) {
      parts.push_back(str.substr(start, end - start));
    }
    start = end + 1;
    end = str.find_first_of(delimiters, start);
  }

  // 添加最后一个token
  if (start < str.length()) {
    parts.push_back(str.substr(start));
  }

  return parts;
}

bool StringUtils::isEnglish(const std::string &str) {
  if (str.empty())
    return false;

  for (char c : str) {
    // 允许字母、撇号和连字符
    if (!isalpha(c) && c != '\'' && c != '-') {
      return false;
    }
  }

  // 确保撇号和连字符不在开头或结尾
  if (str.front() == '\'' || str.front() == '-' || str.back() == '\'' ||
      str.back() == '-') {
    return false;
  }

  return true;
}

bool StringUtils::isChinese(const std::string &str) {
  if (str.empty())
    return false;

  // 常见中文标点符号（Unicode范围）
  static const std::vector<std::pair<uint32_t, uint32_t>>
      chinesePunctuationRanges = {
          {0x3000, 0x303F}, // 中文标点符号
          {0xFF00, 0xFFEF}  // 全角符号
      };

  bool hasChineseChar = false;
  size_t i = 0;
  while (i < str.size()) {
    unsigned char c = str[i];

    // ASCII字符
    if ((c & 0x80) == 0) {
      if (!isspace(c) && !ispunct(c)) {
        return false;
      }
      i++;
    }
    // 多字节字符（可能是中文或中文标点）
    else {
      // 获取完整的UTF-8字符
      uint32_t codePoint = 0;
      int bytes = 0;

      if ((c & 0xE0) == 0xC0) { // 2字节
        codePoint = c & 0x1F;
        bytes = 2;
      } else if ((c & 0xF0) == 0xE0) { // 3字节
        codePoint = c & 0x0F;
        bytes = 3;
      } else if ((c & 0xF8) == 0xF0) { // 4字节
        codePoint = c & 0x07;
        bytes = 4;
      } else {
        return false; // 非法UTF-8序列
      }

      for (int j = 1; j < bytes; ++j) {
        if (i + j >= str.size())
          return false;
        unsigned char next = str[i + j];
        if ((next & 0xC0) != 0x80)
          return false;
        codePoint = (codePoint << 6) | (next & 0x3F);
      }

      // 检查是否是中文（汉字）范围
      bool isChineseChar =
          (codePoint >= 0x4E00 && codePoint <= 0x9FFF) || // 基本汉字
          (codePoint >= 0x3400 && codePoint <= 0x4DBF) || // 扩展A
          (codePoint >= 0x20000 && codePoint <= 0x2FFFF); // 扩展B-F

      // 检查是否是中文标点
      bool isChinesePunct = false;
      for (const auto &range : chinesePunctuationRanges) {
        if (codePoint >= range.first && codePoint <= range.second) {
          isChinesePunct = true;
          break;
        }
      }

      if (isChineseChar) {
        hasChineseChar = true;
      } else if (!isChinesePunct) {
        // 既不是中文也不是中文标点
        return false;
      }

      i += bytes;
    }
  }

  return hasChineseChar;
}

std::string StringUtils::TrimName(std::string name) {
  // 替换下划线为空格
  std::replace(name.begin(), name.end(), '_', ' ');

  // 合并连续空格
  auto new_end = std::unique(name.begin(), name.end(), [](char a, char b) {
    return a == ' ' && b == ' ';
  });
  name.erase(new_end, name.end());

  // 去除首尾空格
  name.erase(name.begin(), std::find_if(name.begin(), name.end(), [](int ch) {
               return !std::isspace(ch);
             }));
  name.erase(std::find_if(name.rbegin(), name.rend(),
                          [](int ch) { return !std::isspace(ch); })
                 .base(),
             name.end());

  return name;
}

} // namespace utils
} // namespace video_scanner
