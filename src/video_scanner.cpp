#include "video_scanner/video_scanner.h"
#include "video_scanner/file_scanner.h"
#include <filesystem>
#include <sys/stat.h>

namespace video_scanner {

VideoScanner::VideoScanner(const std::string& path) 
    : path_(path) {}

std::vector<VideoFileInfo> VideoScanner::Scan() {
    std::vector<VideoFileInfo> result;
    
    // 实现扫描逻辑
    auto files = FileScanner(path_).Scan();
    for (const auto& file : files) {
        // 检查文件扩展名
        if (file.find(".mp4") != std::string::npos || 
            file.find(".avi") != std::string::npos) {
            
            VideoFileInfo info;
            info.filename = std::filesystem::path(file).filename().string();
            info.absolute_path = std::filesystem::absolute(file).string();
            
            struct stat stat_buf;
            stat(file.c_str(), &stat_buf);
            info.size_bytes = stat_buf.st_size;
            
            result.push_back(info);
        }
    }
    
    return result;
}

} // namespace video_scanner