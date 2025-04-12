#include "video_scanner/json_helper.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace video_scanner {

void SaveToJson(const std::vector<std::string> &files,
                const std::string &outputPath) {
  nlohmann::json jsonFiles;
  for (const auto &file : files) {
    jsonFiles.push_back(file);
  }

  std::ofstream outFile(outputPath);
  outFile << jsonFiles.dump(4);
  outFile.close();
}

} // namespace video_scanner