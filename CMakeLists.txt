cmake_minimum_required(VERSION 3.16)
#project name
project(MiniVim)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)

# find & include ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(MiniVim main.cpp)

target_link_libraries(MiniVim ${CURSES_LIBRARY})