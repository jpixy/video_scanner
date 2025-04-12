#include "video_scanner/file_scanner.h"
#include <dirent.h>
#include <iostream>

namespace video_scanner {

FileScanner::FileScanner(const std::string &path) : path_(path) {}

std::vector<std::string> FileScanner::Scan() const {
  std::vector<std::string> files;
  DIR *dir = opendir(path_.c_str());
  if (!dir) {
    std::cerr << "Failed to open directory: " << path_ << std::endl;
    return files;
  }

  dirent *entry;
  while ((entry = readdir(dir)) != nullptr) {
    if (entry->d_type == DT_REG) {
      files.push_back(path_ + "/" + entry->d_name);
    } else if (entry->d_type == DT_DIR && std::string(entry->d_name) != "." &&
               std::string(entry->d_name) != "..") {
      std::string subPath = path_ + "/" + entry->d_name;
      FileScanner subScanner(subPath);
      auto subFiles = subScanner.Scan();
      files.insert(files.end(), subFiles.begin(), subFiles.end());
    }
  }

  closedir(dir);
  return files;
}

} // namespace video_scanner