#include "video_scanner/video_scanner.h"
#include "video_scanner/file_scanner.h"
#include "video_scanner/filename_parser.h"
#include <filesystem>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <algorithm>

namespace video_scanner {

VideoScanner::VideoScanner(const std::string &path) : path_(path) {}

std::vector<VideoFileInfo> VideoScanner::Scan() {
    std::vector<VideoFileInfo> result;

    // 定义支持的视频文件扩展名
    std::vector<std::string> supportedExtensions = {
        ".mp4", ".avi", ".mkv", ".mov", ".wmv", ".flv", ".mpg", ".mpeg", ".m4v", ".3gp"
    };

    // 实现扫描逻辑
    auto files = FileScanner(path_).Scan();
    for (const auto &file : files) {
        // 获取文件扩展名
        std::string extension = std::filesystem::path(file).extension().string();

        // 检查文件扩展名是否在支持的列表中
        if (std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end()) {
            VideoFileInfo info;
            info.filename = std::filesystem::path(file).filename().string();
            info.absolute_path = std::filesystem::absolute(file).string();

            struct stat stat_buf;
            stat(file.c_str(), &stat_buf);
            info.size_bytes = stat_buf.st_size;

            video_scanner::FilenameParser::Parse(info);
            result.push_back(info);
        }
    }

    return result;
}

} // namespace video_scanner