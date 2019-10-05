cmake_minimum_required(VERSION 3.4)
project(ApoapseClientShared LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lstdc++fs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories("../../ApoapseCore/Code/")
include_directories("../../ApoapseCore/ThirdParty/magic_enum/include/")
include_directories("../../ApoapseCore/ThirdParty/cryptopp/")
include_directories("../../ApoapseCore/ThirdParty/boost_1_71_0_LINUX/")
include_directories("../../ApoapseCore/ThirdParty/openssl/include/")

set(OUTPUT_RELEASE ../../BinLinux/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")

# Preprocessor
add_compile_definitions(ENABLE_SECURITY_ASSERTS)
add_compile_definitions(APOAPSE_SERVER)
add_compile_definitions(DYN_LIBRARY)

# Source Files
set(SRC_FILES
    ./ApoapseServer.cpp
    ./Permissions.cpp
    ./ServerCmdManager.cpp
    ./ServerConnection.cpp
    ./ServerFileStreamConnection.cpp
    ./ServerSettings.cpp
    ./ServerSettingsVariables.cpp
    ./stdafx.cpp
    ./ServerEntryPoint.cpp
    ./User.cpp
    ./UsersManager.cpp
    ./UT_User.cpp
)
source_group("Sources" FILES ${SRC_FILES})

# Header Files
set(HEADERS_FILES
    ApoapseServer.h
    Permissions.h
    ServerCmdManager.h
    ServerConnection.h
    ServerEntryPoint.h
    ServerFileStreamConnection.h
    ServerSettings.h
    ServerSettingsVariables.h
    User.h
    UsersManager.h
)
source_group("Headers" FILES ${HEADERS_FILES})

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEADERS_FILES})
#ADD_LIBRARY(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

target_link_libraries(${PROJECT_NAME} stdc++fs)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} dl)

target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libApoapseCore.a")
target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/ApoapseCore/ThirdParty/cryptopp/cryptopp/libcryptopp.a")
target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libssl.a")
target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libcrypto.a")

target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libboost_thread.a")
target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libboost_filesystem.a")
target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libboost_atomic.a")
target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libboost_date_time.a")
target_link_libraries(ApoapseClientShared "/mnt/c/Apoapse/DEV/BinLinux/libboost_system.a")

set_property(TARGET ApoapseClientShared PROPERTY CXX_STANDARD 17)