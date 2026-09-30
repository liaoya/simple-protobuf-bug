cmake_minimum_required(VERSION 3.28)

set(project_name empty-message)
project(${project_name} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # generate compile_commands.json

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
    message(FATAL_ERROR "GCC >= 13 required")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message("Setting default build type to ${CMAKE_BUILD_TYPE}")
endif()

if (MSVC)
    add_compile_options(beast_rest_server PRIVATE /W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(FETCHCONTENT_BASE_DIR
    ${CMAKE_SOURCE_DIR}/external
    CACHE PATH ""
)

include(FetchContent)
FetchContent_Declare(
  simple-protobuf
  GIT_REPOSITORY https://github.com/tonda-kriz/simple-protobuf.git
  GIT_TAG        59c50cee9ac02b75dd45cfedb00ec1a3d6a2e953
)
FetchContent_MakeAvailable(simple-protobuf)

file(GLOB PROTO_FILES "${CMAKE_SOURCE_DIR}/proto/*.proto")
include_directories(src)
add_executable(${project_name}-exe src/main.cpp ${PROTO_FILES})
spb_protobuf_generate(TARGET ${project_name}-exe)
target_link_libraries(${project_name}-exe PRIVATE)
