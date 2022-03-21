cmake_minimum_required(VERSION 3.12.4)
project(string)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/bin")
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

add_executable(StringTest test.cpp String.cpp)
target_link_libraries(StringTest Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})