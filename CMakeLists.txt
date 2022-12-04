cmake_minimum_required(VERSION 3.16)
#project name
project(MiniVim)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)

# find & include ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

#set sources
set(MY_SOURCES
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/fun.cpp
        include/fun.h src/mod.cpp include/mod.h)

add_executable(MiniVim ${MY_SOURCES})

target_link_libraries(MiniVim ${CURSES_LIBRARY})