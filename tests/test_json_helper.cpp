#include "video_scanner/json_helper.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <vector>

// 测试固件类定义
class JsonHelperTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_file_ = "test_output.json";
    }

    void TearDown() override {
        if (std::filesystem::exists(test_file_)) {
            std::filesystem::remove(test_file_);
        }
    }

    std::string test_file_;
};

// 测试用例1：验证正常JSON生成
TEST_F(JsonHelperTest, GeneratesValidJson) {
    std::vector<video_scanner::VideoFileInfo> videos = {
        {"video1.mp4", "/path/to/video1.mp4", 1024},
        {"video2.avi", "/path/to/video2.avi", 2048}
    };
    
    video_scanner::SaveToJson(videos, test_file_);
    
    // 验证文件存在
    ASSERT_TRUE(std::filesystem::exists(test_file_));
    
    // 验证文件内容
    std::ifstream file(test_file_);
    nlohmann::json j;
    file >> j;
    
    EXPECT_EQ(j.size(), 2);
    EXPECT_EQ(j[0]["filename"], "video1.mp4");
    EXPECT_EQ(j[0]["absolute_path"], "/path/to/video1.mp4");
    EXPECT_EQ(j[0]["size_bytes"], 1024);
}

// 测试用例2：验证空列表处理
TEST_F(JsonHelperTest, HandlesEmptyList) {
    std::vector<video_scanner::VideoFileInfo> empty_list;
    video_scanner::SaveToJson(empty_list, test_file_);
    
    ASSERT_TRUE(std::filesystem::exists(test_file_));
    
    std::ifstream file(test_file_);
    nlohmann::json j;
    file >> j;
    
    EXPECT_TRUE(j.is_array());
    EXPECT_TRUE(j.empty());
}