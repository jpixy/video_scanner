#ifndef VIDEO_SCANNER_H
#define VIDEO_SCANNER_H

#include <string>
#include <vector>

namespace video_scanner {

class VideoScanner {
public:
  explicit VideoScanner(const std::string &path);
  std::vector<std::string> Scan() const;

private:
  std::string path_;
};

} // namespace video_scanner

#endif // VIDEO_SCANNER_H