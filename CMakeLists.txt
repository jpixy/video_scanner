cmake_minimum_required(VERSION 3.10)
project(video_scanner)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加包含目录
include_directories(include)

# 添加源文件
add_executable(video_scanner src/main.cpp src/file_scanner.cpp src/video_scanner.cpp src/json_helper.cpp)

# 添加单元测试目标
enable_testing()
add_subdirectory(tests)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# 找到 Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
