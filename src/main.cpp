#include "video_scanner/video_scanner.h"
#include "video_scanner/json_helper.h"
#include <iostream>
#include <filesystem>
#include <iomanip>

void PrintUsage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <input_directory> [output_directory]\n"
              << "  input_directory:  Directory to scan for video files\n"
              << "  output_directory: (Optional) Directory to save JSON results\n"
              << "                    Default: current directory\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        PrintUsage(argv[0]);
        return 1;
    }

    // 获取输入目录
    std::string input_dir = argv[1];
    
    // 设置输出目录（默认为当前目录）
    std::string output_dir = (argc == 3) ? argv[2] : ".";
    
    try {
        // 确保输出目录存在
        if (!std::filesystem::exists(output_dir)) {
            std::filesystem::create_directories(output_dir);
        }

        // 扫描视频文件
        video_scanner::VideoScanner scanner(input_dir);
        auto videoFiles = scanner.Scan();

        // 生成输出文件名（带时间戳）
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << "video_scan_" << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".json";
        std::string output_path = (std::filesystem::path(output_dir) / ss.str()).string();

        // 保存为JSON
        video_scanner::SaveToJson(videoFiles, output_path);

        std::cout << "Scan completed. Found " << videoFiles.size() << " video files.\n"
                  << "Results saved to: " << output_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 