#ifndef FILENAME_PARSER_H
#define FILENAME_PARSER_H

#include "video_scanner/video_types.h"
#include <gtest/gtest_prod.h>
#include <string>
#include <vector>

namespace video_scanner {

// 定义枚举类型，表示语言类型
enum class LanguageType {
  CN,   // 中文
  EN,   // 英文
  OTHER // 其他语言
};

class FilenameParser {
public:
  static void Parse(VideoFileInfo &info);

  /*private:*/
  static bool IsMetaInfo(const std::string &str,
                         const std::vector<std::string> &metaArray);
  static std::vector<std::string>
  FindVideoNames(const std::vector<std::string> &strArray,
                 const std::vector<std::string> &metaArray, LanguageType type);
};

} // namespace video_scanner

#endif // FILENAME_PARSER_H
