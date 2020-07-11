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

# websocket (takuto)
FILE(GLOB ws_api_files websocket/ws_api/*.c)
add_library(ws_api SHARED ${ws_api_files})
target_include_directories(ws_api PRIVATE websocket/ws_api/)
target_link_libraries(ws_api PRIVATE pthread ssl)
target_compile_options(ws_api PRIVATE -Wno-format-truncation -pthread)
FILE(GLOB websocket_files websocket/*.c)
add_library(websocket SHARED ${websocket_files})
target_include_directories(websocket PRIVATE websocket/)
target_link_libraries(websocket PRIVATE ws_api)

FILE(GLOB characters_files characters/*.c)
add_library(characters SHARED ${characters_files})
target_link_libraries(characters PRIVATE ${LIBRARIES})
target_include_directories(characters PRIVATE characters/)
target_include_directories(characters PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.c)
target_link_libraries(main PRIVATE characters websocket)
target_link_libraries(main PRIVATE ${LIBRARIES})
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

