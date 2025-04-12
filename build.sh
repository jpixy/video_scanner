rm -rf ./build
mkdir ./build
cd ./build
cmake -DBUILD_TESTS=ON ..
make

# 运行测试
ctest --output-on-failure