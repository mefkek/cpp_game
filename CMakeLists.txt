cmake_minimum_required(VERSION 3.28)
project(CPP_GAME LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(
    $<$<CONFIG:DEBUG>:-DDEBUG>
    $<$<CONFIG:DEBUG>:-Wall>
    $<$<CONFIG:DEBUG>:-Wextra>
    $<$<CONFIG:DEBUG>:-Wall>
    $<$<CONFIG:DEBUG>:-Wpedantic>
)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_subdirectory(src)