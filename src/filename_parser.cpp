#include "video_scanner/filename_parser.h"
#include <algorithm>
#include <vector>
#include <regex>
#include <cctype>

namespace video_scanner {

void FilenameParser::Parse(VideoFileInfo& file_info) {
    file_info.year = 0;
    file_info.video_name_cn.clear();
    file_info.video_name_en.clear();
    file_info.video_name_other.clear();

    // 1. 首先提取年份
    ExtractYear(file_info.filename, file_info.year);

    // 2. 移除文件扩展名和质量信息
    std::string base_name = RemoveQualityInfo(file_info.filename);

    // 3. 分割文件名各部分
    std::vector<std::string> parts = SplitFilenameParts(base_name);

    // 4. 分析各部分内容
    AnalyzeParts(parts, file_info);
}

bool FilenameParser::ExtractYear(const std::string& filename, int& year) {
    std::regex year_regex(R"((19|20)\d{2})");
    std::smatch match;
    
    if (std::regex_search(filename, match, year_regex)) {
        year = std::stoi(match.str());
        return true;
    }
    return false;
}

std::string FilenameParser::RemoveQualityInfo(const std::string& filename) {
    // 移除文件扩展名
    size_t last_dot = filename.find_last_of('.');
    std::string base_name = (last_dot != std::string::npos) ? 
                           filename.substr(0, last_dot) : filename;

    // 常见质量标记列表
    static const std::vector<std::string> quality_marks = {
        "DC", "HD720P", "HD1080P", "X264", "AAC", "Korean", "CHS", 
        "Mp4Ba", "BluRay", "WEB-DL", "HDR", "REMUX"
    };

    // 移除质量标记
    for (const auto& mark : quality_marks) {
        std::regex mark_regex("\\." + mark + "($|\\.)", std::regex_constants::icase);
        base_name = std::regex_replace(base_name, mark_regex, "");
    }

    return base_name;
}

std::vector<std::string> FilenameParser::SplitFilenameParts(const std::string& filename) {
    std::vector<std::string> parts;
    size_t start = 0;
    size_t end = filename.find('.');

    while (end != std::string::npos) {
        std::string part = filename.substr(start, end - start);
        if (!part.empty()) {
            parts.push_back(part);
        }
        start = end + 1;
        end = filename.find('.', start);
    }

    // 添加最后一部分
    std::string last_part = filename.substr(start);
    if (!last_part.empty()) {
        parts.push_back(last_part);
    }

    return parts;
}

void FilenameParser::AnalyzeParts(const std::vector<std::string>& parts, VideoFileInfo& info) {
    // 1. 首先识别中文部分
    for (const auto& part : parts) {
        if (ContainsChinese(part)) {
            info.video_name_cn = CleanName(part);
            break;
        }
    }

    // 2. 识别英文部分
    for (const auto& part : parts) {
        if (IsEnglish(part) && part != info.video_name_cn) {
            info.video_name_en = CleanName(part);
            break;
        }
    }

    // 3. 处理特殊情况（如"导演剪辑版"等）
    for (const auto& part : parts) {
        if (ContainsChinese(part) && part != info.video_name_cn) {
            if (!info.video_name_cn.empty()) {
                info.video_name_cn += " " + CleanName(part);
            }
        }
    }
}

bool FilenameParser::ContainsChinese(const std::string& str) {
    return std::any_of(str.begin(), str.end(), [](char c) {
        return (c & 0x80) != 0;
    });
}

bool FilenameParser::IsEnglish(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char c) {
        return isalpha(c) || isspace(c) || c == '-';
    });
}

std::string FilenameParser::CleanName(std::string name) {
    // 替换分隔符
    std::replace(name.begin(), name.end(), '_', ' ');
    
    // 移除多余空格
    std::regex space_regex("\\s+");
    name = std::regex_replace(name, space_regex, " ");
    
    // 去除首尾空格
    name.erase(0, name.find_first_not_of(" "));
    name.erase(name.find_last_not_of(" ") + 1);
    
    return name;
}

} // namespace video_scanner