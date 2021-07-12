cmake_minimum_required(VERSION 3.0)
project(PlayerConverter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(extern/portaudio)

add_library(audio_data STATIC src/audio_data.cc)
target_include_directories(audio_data PUBLIC include)

add_library(audio_data_player OBJECT src/audio_data_player.cc)
target_link_libraries(audio_data_player PUBLIC audio_data)
target_include_directories(audio_data_player PUBLIC include)

add_executable(player-converter src/main.cc)
target_link_libraries(player-converter PUBLIC portaudio_static
                                              audio_data
                                              audio_data_player
                                              dl
                                              pthread)
target_include_directories(player-converter PUBLIC "${PROJECT_BINARY_DIR}"
                                                   include
                                                   extern/portaudio)


add_subdirectory(src/formats)
