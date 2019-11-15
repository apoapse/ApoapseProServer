cmake_minimum_required(VERSION 3.4)
project(ServerConsole LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec")

include_directories("../../ApoapseCore/Code/")
include_directories("../ApoapseServerShared/")

set(OUTPUT_RELEASE ../../BinLinux/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")

# Source Files
set(SRC_FILES
    ./main.cpp
)
source_group("Sources" FILES ${SRC_FILES})

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

target_link_libraries(ServerConsole "/mnt/c/Apoapse/DEV/BinLinux/libApoapseServerShared.so")

set_property(TARGET ServerConsole PROPERTY CXX_STANDARD 17)