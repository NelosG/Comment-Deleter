cmake_minimum_required(VERSION 3.19)
project(Deleter)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(suffix -release)
else()
    set(suffix -debug)
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build${suffix})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build${suffix})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build${suffix})


set(Utils_Sources Utils/file_utils.cpp Utils/file_utils.h)

set(Comment_Deleter_Sources comment_deleter.h comment_deleter.cpp)

add_library(comment_deleter-static STATIC ${Comment_Deleter_Sources} ${Utils_Sources})

add_library(comment_deleter-shared SHARED ${Comment_Deleter_Sources} ${Utils_Sources})

target_include_directories(comment_deleter-static
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Utils>)

target_include_directories(comment_deleter-shared
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Utils>)

add_custom_command(
        TARGET comment_deleter-static
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/comment_deleter.h ${CMAKE_SOURCE_DIR}/build${suffix}
)

add_custom_command(
        TARGET comment_deleter-shared
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/comment_deleter.h ${CMAKE_SOURCE_DIR}/build${suffix}
)
