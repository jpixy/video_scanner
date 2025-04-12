#include "video_scanner/json_helper.h"
#include "video_scanner/video_scanner.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path>" << std::endl;
    return 1;
  }

  std::string path = argv[1];
  video_scanner::VideoScanner scanner(path);
  auto videoFiles = scanner.Scan();

  std::string outputJsonPath = "output.json";
  video_scanner::SaveToJson(videoFiles, outputJsonPath);

  std::cout << "Scanning completed. Results saved to " << outputJsonPath
            << std::endl;
  return 0;
}