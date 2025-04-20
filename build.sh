#!/bin/bash

# 删除旧的构建目录
rm -rf ./build
mkdir ./build

# # 设置 vcpkg 路径
# VCPKG_DIR="${PWD}/vcpkg"

# # 设置 VCPKG_ROOT 环境变量
# export VCPKG_ROOT="$VCPKG_DIR"

# # 检查 vcpkg 是否存在
# if [ ! -d "$VCPKG_DIR" ]; then
#     echo "Cloning vcpkg..."
#     git clone https://github.com/microsoft/vcpkg.git
#     cd vcpkg
#     ./bootstrap-vcpkg.sh
#     cd ..
# fi

# # 安装依赖
# "$VCPKG_DIR/vcpkg" install spdlog fmt gtest

# 生成构建文件
cmake -B build -S . -DBUILD_TESTS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DSPDLOG_FMT_EXTERNAL=OFF

# 编译项目
cmake --build build

# 运行测试
cd build
ctest --output-on-failure
cd ..
