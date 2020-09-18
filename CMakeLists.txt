cmake_minimum_required(VERSION 3.16)
project(flickr_dataset_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

# opencv
find_package(OpenCV 3.4.11 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# curl
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

# Qt
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# json
include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(flickr_dataset
        main.cpp
        utils/config.cpp
        utils/console.cpp
        utils/formatter.cpp
        utils/inih/ini.c
        components/download.cpp
        components/check.cpp)
target_link_libraries(flickr_dataset
        ${OpenCV_LIBS}
        ${CURL_LIBRARIES}
        Qt5::Widgets
        nlohmann_json::nlohmann_json)
