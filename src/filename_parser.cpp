#include "video_scanner/filename_parser.h"
#include "video_scanner/logger.h"
#include <algorithm>
#include <regex>
#include <cctype>

namespace video_scanner {

void FilenameParser::Parse(VideoFileInfo& file_info) {
    video_scanner::Logger::init();
    LOG_INFO("Starting to parse filename: {}", file_info.filename);
    
    file_info.year = 0;
    file_info.video_name_cn.clear();
    file_info.video_name_en.clear();
    file_info.video_name_other.clear();

    // 1. 去除后缀名
    std::string base_name = RemoveQualityInfo(file_info.filename);

    // 2. 查找文件名中是否有年份
    ExtractYear(base_name, file_info.year);

    // 3. 如果有年份，以年份为分割点
    std::vector<std::string> parts = SplitFilenameParts(base_name);
    int year_index = -1;
    for (size_t i = 0; i < parts.size(); ++i) {
        int possible_year = 0;
        if (ExtractYear(parts[i], possible_year)) {
            year_index = static_cast<int>(i);
            break;
        }
    }

    // 4. 根据是否找到年份，应用逻辑A
    if (year_index != -1) {
        std::vector<std::string> before_year(parts.begin(), parts.begin() + year_index);
        std::vector<std::string> after_year(parts.begin() + year_index + 1, parts.end());
        ApplyLogicA(before_year, file_info);
        ApplyLogicA(after_year, file_info);
    } else {
        ApplyLogicA(parts, file_info);
    }

    // 清理前后空格
    file_info.video_name_cn = CleanName(file_info.video_name_cn);
    file_info.video_name_en = CleanName(file_info.video_name_en);
    file_info.video_name_other = CleanName(file_info.video_name_other);
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
    size_t last_dot = filename.find_last_of('.');
    return (last_dot != std::string::npos) ? filename.substr(0, last_dot) : filename;
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

    std::string last_part = filename.substr(start);
    if (!last_part.empty()) {
        parts.push_back(last_part);
    }

    return parts;
}

void FilenameParser::ApplyLogicA(const std::vector<std::string>& parts, VideoFileInfo& info) {
    std::string tmp;
    for (size_t i = 0; i < parts.size(); ++i) {
        const std::string& part = parts[i];

        // 跳过元信息
        if (IsMetaInfo(part)) {
            if (!tmp.empty()) {
                ApplyLogicB(tmp, info);
                tmp.clear();
            }
            continue;
        }

        // 拼接连续的中文、英文或其他语言单词
        if (!tmp.empty()) tmp += " ";
        tmp += part;

        // 检查是否是最后一个部分或下一个部分是元信息
        if (i == parts.size() - 1 || IsMetaInfo(parts[i + 1])) {
            ApplyLogicB(tmp, info);
            tmp.clear();
        }
    }
}

void FilenameParser::ApplyLogicB(std::string& tmp, VideoFileInfo& info) {
    // 1. 判断 tmp 后面是否直接跟着数字
    std::regex number_suffix_regex(R"((\s|\.|_|-)(\d+)$)");
    std::smatch match;
    if (std::regex_search(tmp, match, number_suffix_regex)) {
        tmp += " " + match.str(2);
    }

    // 2. 如果 tmp 是中文，删除字母加下划线前缀
    if (ContainsChinese(tmp)) {
        tmp = CleanChinesePrefix(tmp);
    }

    // 3. 判断 tmp 是中文、英文还是其他语言
    if (ContainsChinese(tmp)) {
        info.video_name_cn = tmp;
    } else if (IsEnglish(tmp)) {
        info.video_name_en = tmp;
    } else {
        info.video_name_other = tmp;
    }
}

bool FilenameParser::IsMetaInfo(const std::string& part) {
    static const std::vector<std::string> meta_keywords = {
        "DC", "HD720P", "HD1080P", "X264", "AAC", "Korean", "CHS", "CHT",
        "BluRay", "WEB-DL", "HDR", "REMUX", "DTS", "AC3", "H265", "HEVC",
        "AAC5.1", "DDP5.1", "Atmos", "SDR", "DV", "HDR10", "HDR10+", "IMAX",
        "4K", "60帧", "KORSUB", "WEBRip", "H264", "H265", "2Audio",
        "国语中字", "中简繁英字幕"
    };

    for (const auto& kw : meta_keywords) {
        if (part.find(kw) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string FilenameParser::CleanChinesePrefix(const std::string& name) {
    std::regex prefix_regex(R"(^[A-Za-z_]+)");
    return std::regex_replace(name, prefix_regex, "");
}

bool FilenameParser::ContainsChinese(const std::string& str) {
    return std::any_of(str.begin(), str.end(), [](char c) {
        return (c & 0x80) != 0;
    });
}

bool FilenameParser::IsEnglish(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char c) {
        return isalpha(c) || isspace(c) || c == '-' || c == '\'';
    });
}

std::string FilenameParser::CleanName(std::string name) {
    std::replace(name.begin(), name.end(), '_', ' ');
    std::regex space_regex("\\s+");
    name = std::regex_replace(name, space_regex, " ");
    name.erase(0, name.find_first_not_of(" "));
    name.erase(name.find_last_not_of(" ") + 1);
    return name;
}

} // namespace video_scanner