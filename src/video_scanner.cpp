#include "video_scanner/video_scanner.h"
#include "video_scanner/file_scanner.h"
#include <algorithm>
#include <string>

namespace video_scanner {

VideoScanner::VideoScanner(const std::string &path) : path_(path) {}

std::vector<std::string> VideoScanner::Scan() const {
  FileScanner scanner(path_);
  std::vector<std::string> allFiles = scanner.Scan();
  std::vector<std::string> videoFiles;

  for (const auto &file : allFiles) {
    std::string ext = file.substr(file.find_last_of(".") + 1);
    if (ext == "mp4" || ext == "avi" || ext == "mkv" || ext == "mov") {
      videoFiles.push_back(file);
    }
  }

  return videoFiles;
}

} // namespace video_scanner