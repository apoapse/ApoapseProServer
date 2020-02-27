cmake_minimum_required(VERSION 3.4)
project(ApoapseServerShared LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")

include_directories("../../ApoapseProtocol/Code/")
include_directories("../../ApoapseProtocol/ThirdParty/magic_enum/include/")
include_directories("../../ApoapseProtocol/ThirdParty/cryptopp/")
include_directories("../../ApoapseProtocol/ThirdParty/boost/")
include_directories("../../ApoapseProtocol/ThirdParty/openssl/include/")

set(OUTPUT_RELEASE ../../Bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${OUTPUT_RELEASE}")

# Preprocessor
add_compile_definitions(ENABLE_SECURITY_ASSERTS)
add_compile_definitions(APOAPSE_SERVER)
add_compile_definitions(DYN_LIBRARY)
add_compile_definitions(DEBUG)

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
target_link_libraries(${PROJECT_NAME} -static-libstdc++)

target_link_libraries(ApoapseServerShared "../../Bin/libApoapseProtocol.a")
target_link_libraries(ApoapseServerShared "../ApoapseProtocol/ThirdParty/cryptopp/cryptopp/libcryptopp.a")
target_link_libraries(ApoapseServerShared "../../Bin/libssl.a")
target_link_libraries(ApoapseServerShared "../../Bin/libcrypto.a")

target_link_libraries(ApoapseServerShared "../../Bin/libboost_thread.a")
target_link_libraries(ApoapseServerShared "../../Bin/libboost_filesystem.a")
target_link_libraries(ApoapseServerShared "../../libboost_atomic.a")
target_link_libraries(ApoapseServerShared "../../libboost_date_time.a")
target_link_libraries(ApoapseServerShared "../../libboost_system.a")

set_property(TARGET ApoapseServerShared PROPERTY CXX_STANDARD 17)