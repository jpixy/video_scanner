#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <string>
#include <vector>

namespace video_scanner {

class FileScanner {
public:
  explicit FileScanner(const std::string &path);
  std::vector<std::string> Scan() const;

private:
  std::string path_;
};

} // namespace video_scanner

#endif // FILE_SCANNER_H