#ifndef VIDEO_SCANNER_VIDEO_TYPES_H
#define VIDEO_SCANNER_VIDEO_TYPES_H

#include <string>
#include <vector>
#include <cstdint>  // 添加此头文件以定义 uint64_t

namespace video_scanner {

struct VideoFileInfo {
    std::string filename;
    std::string absolute_path;
    uint64_t size_bytes;
};

} // namespace video_scanner

#endif // VIDEO_SCANNER_VIDEO_TYPES_H