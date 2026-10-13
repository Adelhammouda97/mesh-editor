cmake_minimum_required(VERSION 3.20)
project(MeshEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core library: pure topology, no graphics dependency
add_library(meshcore
    src/core/Mesh.cpp
)
target_include_directories(meshcore PUBLIC src)
target_compile_options(meshcore PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>)

# Tests (Catch2 is downloaded automatically on first configure)
include(FetchContent)
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2)
FetchContent_MakeAvailable(Catch2)

enable_testing()
add_executable(tests tests/test_halfedge.cpp)
target_link_libraries(tests PRIVATE meshcore Catch2::Catch2WithMain)
add_test(NAME tests COMMAND tests)