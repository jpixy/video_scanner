cmake_minimum_required(VERSION 3.10)
project(video_scanner)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 关键设置：强制使用 spdlog 自带的 fmt
set(SPDLOG_FMT_EXTERNAL OFF) 
# 查找spdlog
find_package(spdlog REQUIRED)

# 添加包含目录
include_directories(include)

# 添加源文件
add_executable(video_scanner 
	src/main.cpp 
	src/logger.cpp
	src/file_scanner.cpp 
	src/video_scanner.cpp
	src/json_helper.cpp 
	src/filename_parser.cpp
)

# 链接库
target_link_libraries(video_scanner 
    PRIVATE 
	spdlog::spdlog_header_only
    stdc++fs
)

# 添加单元测试目标
enable_testing()
add_subdirectory(tests)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# 创建日志目录
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/logs)

# 找到 Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
