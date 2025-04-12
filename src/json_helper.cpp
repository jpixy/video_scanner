#include "video_scanner/json_helper.h"
#include <fstream>

namespace video_scanner {

void SaveToJson(const std::vector<VideoFileInfo>& videos, 
               const std::string& output_path) {
    nlohmann::json j = nlohmann::json::array(); // 显式创建空数组
    
    for (const auto& video : videos) {
        j.push_back({
            {"filename", video.filename},
            {"absolute_path", video.absolute_path},
            {"size_bytes", video.size_bytes}
        });
    }
    
    std::ofstream out(output_path);
    if (out) {
        out << j.dump(4);
    }
}

} // namespace video_scanner