#ifndef VIDEO_SCANNER_UTILS_STRING_UTILS_H
#define VIDEO_SCANNER_UTILS_STRING_UTILS_H

#include <string>
#include <vector>

namespace video_scanner {
namespace utils {

class StringUtils {
public:
  static std::string RemoveExtension(const std::string &filename);
  static bool ExtractMovieYear(const std::string &filename, int &year);
  static std::pair<std::string, std::string>
  SplitStringByDelimitersToPair(const std::string &str,
                                const std::string &delimiters);
  static std::vector<std::string>
  SplitStringByDelimitersToArray(const std::string &str,
                                 const std::string &delimiters);
  static bool isEnglish(const std::string &str);
  static bool isChinese(const std::string &str);
  static std::string TrimName(std::string name);

  // 禁止实例化
  StringUtils() = delete;
};

} // namespace utils
} // namespace video_scanner

#endif // VIDEO_SCANNER_UTILS_STRING_H
