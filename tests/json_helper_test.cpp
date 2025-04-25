#include "video_scanner/json_helper.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

class JsonHelperTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 使用临时目录作为测试目录
        test_output_dir_ = std::filesystem::temp_directory_path() / "video_scanner_test";
        std::filesystem::create_directory(test_output_dir_);
    }

    void TearDown() override {
        // 清理测试目录
        std::filesystem::remove_all(test_output_dir_);
    }

    std::filesystem::path test_output_dir_;
};

TEST_F(JsonHelperTest, GeneratesValidJson) {
    std::vector<video_scanner::VideoFileInfo> videos = {
        {"video1.mp4", "/path/to/video1.mp4", 1024, "Test Movie", "", "", 2023}
    };
    
    std::string output_file = (test_output_dir_ / "output.json").string();
    video_scanner::SaveToJson(videos, output_file);
    
    // 验证文件存在
    ASSERT_TRUE(std::filesystem::exists(output_file));
    
    // 验证文件内容
    std::ifstream file(output_file);
    nlohmann::json j;
    file >> j;
    
    EXPECT_TRUE(j.is_array());
    EXPECT_EQ(j.size(), 1);
    EXPECT_EQ(j[0]["filename"], "video1.mp4");
}

TEST_F(JsonHelperTest, HandlesEmptyList) {
    std::vector<video_scanner::VideoFileInfo> empty_list;
    std::string output_file = (test_output_dir_ / "empty.json").string();
    
    video_scanner::SaveToJson(empty_list, output_file);
    
    // 验证文件存在
    ASSERT_TRUE(std::filesystem::exists(output_file));
    
    // 验证文件内容
    std::ifstream file(output_file);
    nlohmann::json j;
    file >> j;
    
    EXPECT_TRUE(j.is_array());
    EXPECT_TRUE(j.empty());
    EXPECT_EQ(j.dump(), "[]");
}