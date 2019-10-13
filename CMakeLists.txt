cmake_minimum_required(VERSION 3.15)
project(glyph-loading VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)

set(THIRD_PARTY_DIR "third_party/")
include_directories(${THIRD_PARTY_DIR})
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)