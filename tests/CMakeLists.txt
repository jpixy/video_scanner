cmake_minimum_required(VERSION 3.10)
project(video_scanner_tests)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找GoogleTest
find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

# 定义公共链接库
set(COMMON_LIBS
    GTest::GTest 
    GTest::Main
    Threads::Threads
    stdc++fs
    spdlog::spdlog_header_only
)

# 添加包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

# 添加源文件路径
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# 添加测试可执行文件
add_executable(file_scanner_test 
    file_scanner_test.cpp
    ${SRC_DIR}/file_scanner.cpp
    ${SRC_DIR}/logger.cpp
)

add_executable(filename_parser_test
    filename_parser_test.cpp
    ${SRC_DIR}/filename_parser.cpp
    ${SRC_DIR}/logger.cpp
    ${SRC_DIR}/utils/string_utils.cpp
)

add_executable(video_scanner_test
    video_scanner_test.cpp
    ${SRC_DIR}/video_scanner.cpp
    ${SRC_DIR}/file_scanner.cpp
    ${SRC_DIR}/filename_parser.cpp
    ${SRC_DIR}/logger.cpp
    ${SRC_DIR}/utils/string_utils.cpp
)

add_executable(json_helper_test
    json_helper_test.cpp
    ${SRC_DIR}/json_helper.cpp
    ${SRC_DIR}/logger.cpp
)

add_executable(string_utils_test
    utils/string_utils_test.cpp
    ${SRC_DIR}/utils/string_utils.cpp
    ${SRC_DIR}/logger.cpp
)

# 链接库
target_link_libraries(file_scanner_test 
    PRIVATE
    ${COMMON_LIBS}
)

target_link_libraries(filename_parser_test 
    PRIVATE
    ${COMMON_LIBS}
)

target_link_libraries(video_scanner_test 
    PRIVATE
    ${COMMON_LIBS}
)

target_link_libraries(json_helper_test
    PRIVATE
    ${COMMON_LIBS}
)

target_link_libraries(string_utils_test
    PRIVATE
    ${COMMON_LIBS}
)

# 添加测试
enable_testing()
add_test(NAME file_scanner_test COMMAND file_scanner_test)
add_test(NAME filename_parser_test COMMAND filename_parser_test)
add_test(NAME video_scanner_test COMMAND video_scanner_test)
add_test(NAME json_helper_test COMMAND json_helper_test)
add_test(NAME string_utils_test COMMAND string_utils_test)

