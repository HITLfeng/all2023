# 指定CMake最低版本 建议写
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 指定项目名称 （版本 备注 语言...）
project(testDemo)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOME /root/project/all2023/kv-database)
set(EXECUTABLE_OUTPUT_PATH ${HOME}/test)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb") 
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(${HOME}/output/include)
link_directories(${HOME}/output/lib)


file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
# 设置要依赖的动态库

# 指定可执行程序的名称及依赖


add_executable(main main.cc ${SRC_LIST})

target_link_libraries(main gtest pthread)
# 执行 cmake + 路径 （CMakeLists.txt 文件所在路径）