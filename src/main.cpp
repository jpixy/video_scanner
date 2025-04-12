#include "video_scanner/video_scanner.h"
#include "video_scanner/json_helper.h"
#include <iostream>
#include <chrono>
#include <iomanip>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        return 1;
    }

    try {
        // 扫描视频文件
        video_scanner::VideoScanner scanner(argv[1]);
        auto videoFiles = scanner.Scan();  // 现在应该返回 VideoFileInfo 向量

        // 生成输出文件名
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << "video_scan_" << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".json";
        std::string outputJsonPath = ss.str();

        // 保存为JSON
        video_scanner::SaveToJson(videoFiles, outputJsonPath);

        std::cout << "Scan completed. Found " << videoFiles.size() << " video files." << std::endl;
        std::cout << "Results saved to: " << outputJsonPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}