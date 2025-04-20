cmake_minimum_required(VERSION 3.10)
project(video_scanner_tests)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找GoogleTest
find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

# 添加包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

# 添加源文件路径
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# 添加测试可执行文件
add_executable(test_file_scanner 
    test_file_scanner.cpp
    ${SRC_DIR}/file_scanner.cpp
    ${SRC_DIR}/logger.cpp
)

add_executable(test_filename_parser
    test_filename_parser.cpp
    ${SRC_DIR}/filename_parser.cpp
    ${SRC_DIR}/logger.cpp
)

add_executable(test_video_scanner 
    test_video_scanner.cpp
    ${SRC_DIR}/video_scanner.cpp
    ${SRC_DIR}/file_scanner.cpp
    ${SRC_DIR}/filename_parser.cpp
    ${SRC_DIR}/logger.cpp
)

add_executable(test_json_helper
    test_json_helper.cpp
    ${SRC_DIR}/json_helper.cpp
    ${SRC_DIR}/logger.cpp
)

# 链接库
target_link_libraries(test_file_scanner 
    PRIVATE
    GTest::GTest 
    GTest::Main
    Threads::Threads
    stdc++fs
    spdlog::spdlog_header_only
)

target_link_libraries(test_filename_parser 
    PRIVATE
    GTest::GTest 
    GTest::Main
    Threads::Threads
    stdc++fs
    spdlog::spdlog_header_only
)

target_link_libraries(test_video_scanner 
    PRIVATE
    GTest::GTest 
    GTest::Main
    Threads::Threads
    stdc++fs
    spdlog::spdlog_header_only
)

target_link_libraries(test_json_helper
    PRIVATE
    GTest::GTest 
    GTest::Main
    Threads::Threads
    stdc++fs
    spdlog::spdlog_header_only
)

# 添加测试
enable_testing()
add_test(NAME test_file_scanner COMMAND test_file_scanner)
add_test(NAME test_filename_parser COMMAND test_filename_parser)
add_test(NAME test_video_scanner COMMAND test_video_scanner)
add_test(NAME test_json_helper COMMAND test_json_helper)