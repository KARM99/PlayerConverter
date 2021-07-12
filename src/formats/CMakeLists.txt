cmake_minimum_required(VERSION 3.0)
project(format_libraries)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(supported_formats aiff wav)

foreach(format IN LISTS supported_formats)
    add_library(${format} SHARED ${format}.cc)
    target_link_libraries(${format} PUBLIC ${PROJECT_SOURCE_DIR}/../../build/libaudio_data.a)
    target_include_directories(${format} PUBLIC ../../include)
endforeach()
