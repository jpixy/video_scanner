#ifndef VIDEO_SCANNER_VIDEO_TYPES_H
#define VIDEO_SCANNER_VIDEO_TYPES_H

#include <cstdint> // 添加此头文件以定义 uint64_t
#include <string>
#include <vector>

namespace video_scanner {

struct VideoFileInfo {
  std::string filename;
  std::string absolute_path;
  uint64_t size_bytes;

  std::string video_name_en; // 英文名
  std::string video_name_cn; // 中文名
  std::string video_name_other;
  int year;
};

} // namespace video_scanner

#endif // VIDEO_SCANNER_VIDEO_TYPES_H