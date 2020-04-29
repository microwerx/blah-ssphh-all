cmake_minimum_required(VERSION 3.13)
project(ssphh-all)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/../imgui)

add_subdirectory(${PROJECT_SOURCE_DIR}/../fluxions fluxions)
add_subdirectory(ssphh)
#add_subdirectory(ssphh-tests)
