#include "video_scanner/file_scanner.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

class FileScannerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_dir_ = std::filesystem::current_path() / "test_data";
        std::filesystem::create_directory(test_dir_);
        
        // 创建测试文件
        std::ofstream(test_dir_ / "video.mp4");
        std::ofstream(test_dir_ / "video.avi");
        std::ofstream(test_dir_ / "text.txt");
    }

    void TearDown() override {
        std::filesystem::remove_all(test_dir_);
    }

    std::filesystem::path test_dir_;
};

TEST_F(FileScannerTest, ScannerInitialization) {
    EXPECT_NO_THROW({
        video_scanner::FileScanner scanner(test_dir_.string());
    });
}

TEST_F(FileScannerTest, ScanFindsFiles) {
    video_scanner::FileScanner scanner(test_dir_.string());
    auto files = scanner.Scan();
    
    // 应该找到3个文件（包括非视频文件）
    EXPECT_EQ(files.size(), 3);
    
    // 验证包含特定文件名
    bool found_mp4 = false;
    bool found_avi = false;
    bool found_txt = false;
    
    for (const auto& file : files) {
        if (file.find("video.mp4") != std::string::npos) found_mp4 = true;
        if (file.find("video.avi") != std::string::npos) found_avi = true;
        if (file.find("text.txt") != std::string::npos) found_txt = true;
    }
    
    EXPECT_TRUE(found_mp4);
    EXPECT_TRUE(found_avi);
    EXPECT_TRUE(found_txt);
}

TEST_F(FileScannerTest, HandlesEmptyDirectory) {
    std::filesystem::path empty_dir = test_dir_ / "empty";
    std::filesystem::create_directory(empty_dir);
    
    video_scanner::FileScanner scanner(empty_dir.string());
    auto files = scanner.Scan();
    
    EXPECT_TRUE(files.empty());
    
    std::filesystem::remove_all(empty_dir);
}