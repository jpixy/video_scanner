#ifndef VIDEO_SCANNER_H
#define VIDEO_SCANNER_H

#include "video_scanner/video_types.h"
#include "video_scanner/file_scanner.h"

namespace video_scanner {

class VideoScanner {
public:
    explicit VideoScanner(const std::string& path);
    std::vector<VideoFileInfo> Scan();
    
private:
    std::string path_;
};

} // namespace video_scanner

#endif // VIDEO_SCANNER_H