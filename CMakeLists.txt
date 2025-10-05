cmake_minimum_required(VERSION 3.10)
project(VolCurveVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(main
    main.cpp
    DataFetcher.cpp
)

find_package(CURL REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)
