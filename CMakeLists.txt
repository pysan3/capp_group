cmake_minimum_required (VERSION 3.0)

project(hoge)

if (APPLE)
    # add_compile_options("SHELL:-framework GULT" "SHELL:-framework OpenGL" -mmacosx-version-min=10.8)
    add_definitions(-Wno-deprecated-declarations)
else()
    link_libraries(glut GLU GL)
endif()

link_libraries(m)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
set(LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
include_directories(${INCLUDE_DIRS})

FILE(GLOB player_files Player/*.c)
add_library(player SHARED ${player_files})
target_include_directories(player PRIVATE Player/)

FILE(GLOB characters_files characters/*.c)
add_library(characters SHARED ${characters_files})
target_link_libraries(characters PRIVATE ${LIBRARIES})
target_include_directories(characters PRIVATE characters/)
target_include_directories(characters PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.c)
target_link_libraries(main PRIVATE characters)
target_link_libraries(main PRIVATE ${LIBRARIES})
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
