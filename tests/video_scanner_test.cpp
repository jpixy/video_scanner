#include "video_scanner/video_scanner.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

// 测试固件类
class VideoScannerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir_ = std::filesystem::current_path() / "test_data";
        std::filesystem::create_directory(test_dir_);
        
        // 创建测试文件
        {
            std::ofstream(test_dir_ / "video.mp4");
            std::ofstream(test_dir_ / "video.avi");
            std::ofstream(test_dir_ / "text.txt");
        }
    }

    void TearDown() override {
        std::filesystem::remove_all(test_dir_);
    }

    std::filesystem::path test_dir_;
};

// 测试用例1：验证扫描器初始化
TEST_F(VideoScannerTest, ScannerInitialization) {
    EXPECT_NO_THROW({
        video_scanner::VideoScanner scanner(test_dir_.string());
    });
}

// 测试用例2：验证能找到视频文件
TEST_F(VideoScannerTest, ScanFindsVideoFiles) {
    video_scanner::VideoScanner scanner(test_dir_.string());
    auto videoFiles = scanner.Scan();
    
    // 验证找到的视频文件
    bool found_mp4 = false;
    bool found_avi = false;
    
    for (const auto& fileInfo : videoFiles) {
        if (fileInfo.filename == "video.mp4") found_mp4 = true;
        if (fileInfo.filename == "video.avi") found_avi = true;
    }
    
    EXPECT_TRUE(found_mp4) << "MP4 file not found";
    EXPECT_TRUE(found_avi) << "AVI file not found";
}

// 测试用例3：验证处理空目录
TEST_F(VideoScannerTest, HandlesEmptyDirectory) {
    std::filesystem::path empty_dir = test_dir_ / "empty";
    std::filesystem::create_directory(empty_dir);
    
    video_scanner::VideoScanner scanner(empty_dir.string());
    auto files = scanner.Scan();
    
    EXPECT_TRUE(files.empty()) << "Non-empty result from empty directory";
    
    std::filesystem::remove_all(empty_dir);
}