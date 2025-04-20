#ifndef FILENAME_PARSER_H
#define FILENAME_PARSER_H

#include "video_scanner/video_types.h"
#include <string>
#include <vector>

namespace video_scanner {

class FilenameParser {
public:
    static void Parse(VideoFileInfo& info);

private:
    static bool ExtractYear(const std::string& filename, int& year);
    static std::string RemoveQualityInfo(const std::string& filename);
    static std::vector<std::string> SplitFilenameParts(const std::string& filename);
    static void AnalyzeParts(const std::vector<std::string>& parts, VideoFileInfo& info);
    static void ApplyLogicA(const std::vector<std::string>& parts, VideoFileInfo& info);
    static void ApplyLogicB(std::string& tmp, VideoFileInfo& info);
    static bool IsMetaInfo(const std::string& part);
    static bool ContainsChinese(const std::string& str);
    static bool IsEnglish(const std::string& str);
    static std::string CleanName(std::string name);
    static std::string CleanChinesePrefix(const std::string& name);
};

} // namespace video_scanner

#endif // FILENAME_PARSER_H