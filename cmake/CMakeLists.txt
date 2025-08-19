cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(hellocmake 
    LANGUAGES CXX 
    VERSION 0.1.0
    DESCRIPTION "A cmake, spdlog, fmt, cli11 test project"
)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)

add_executable(main)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
aux_source_directory(src sources)
target_sources(main PUBLIC ${sources})

target_link_libraries(main PRIVATE fmt)
target_link_libraries(main PRIVATE spdlog)
target_link_libraries(main PRIVATE CLI11::CLI11)
