cmake_minimum_required(VERSION 3.10)
project(glyph-loading VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)

find_package(Freetype REQUIRED)
if(NOT FREETYPE_FOUND)
    message(Error  "Freetype not found")
endif(NOT FREETYPE_FOUND)

set(THIRD_PARTY_DIR "third_party/" ${FREETYPE_INCLUDE_DIRS})
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/bitmap.h src/glyph.h src/font_face.cpp src/font_face.h src/freetype_face.h src/freetype_face.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${THIRD_PARTY_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${FREETYPE_LIBRARY})
