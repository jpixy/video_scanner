#ifndef VIDEO_SCANNER_JSON_HELPER_H
#define VIDEO_SCANNER_JSON_HELPER_H

#include "video_scanner/video_types.h"
#include <nlohmann/json.hpp>

namespace video_scanner {

void SaveToJson(const std::vector<VideoFileInfo>& videos, 
               const std::string& output_path);

} // namespace video_scanner

#endif // VIDEO_SCANNER_JSON_HELPER_H