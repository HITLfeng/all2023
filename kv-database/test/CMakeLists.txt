# 指定CMake最低版本 建议写
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 指定项目名称 （版本 备注 语言...）
project(firstdemo)

set(HOME /root/project/all2023/kv-database)
set(EXECUTABLE_OUTPUT_PATH ${HOME}/test)

# 指定可执行程序的名称及依赖
add_executable(main main.cc)
target_link_libraries(main gtest pthread)
# 执行 cmake + 路径 （CMakeLists.txt 文件所在路径）