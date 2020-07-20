cmake_minimum_required (VERSION 3.0)

project(hoge)

link_libraries(glut GLU GL m)

FILE(GLOB characters_files characters/*.c)
add_library(characters SHARED ${characters_files})
target_include_directories(characters PRIVATE characters/)
target_include_directories(characters PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.c)
target_link_libraries(main PRIVATE characters)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
