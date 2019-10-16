cmake_minimum_required(VERSION 3.10)
project(glyph-loading VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)

find_package(Freetype REQUIRED)
if(NOT FREETYPE_FOUND)
    message(Error  "Freetype not found")
endif(NOT FREETYPE_FOUND)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive")
set(THIRD_PARTY_DIR "third_party/" ${FREETYPE_INCLUDE_DIRS})
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.h")
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${THIRD_PARTY_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${FREETYPE_LIBRARY})
