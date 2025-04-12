#include "video_scanner/json_helper.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace video_scanner {

void SaveToJson(const std::vector<VideoFileInfo>& videos, 
               const std::string& output_path) {
    // 确保输出目录存在
    std::filesystem::path path(output_path);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }
    
    // 显式创建 JSON 数组
    nlohmann::json j = nlohmann::json::array();  // 确保始终是数组
    
    for (const auto& video : videos) {
        j.push_back({
            {"filename", video.filename},
            {"absolute_path", video.absolute_path},
            {"size_bytes", video.size_bytes},
            {"video_name_en", video.video_name_en},
            {"video_name_cn", video.video_name_cn},
            {"video_name_other", video.video_name_other},
            {"year", video.year}
        });
    }
    
    std::ofstream out(output_path);
    if (out) {
        out << j.dump(4);
    } else {
        throw std::runtime_error("Failed to open output file: " + output_path);
    }
}

} // namespace video_scanner