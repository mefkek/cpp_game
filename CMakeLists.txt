cmake_minimum_required(VERSION 3.28)
project(CPP_GAME LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(epic_dungeon src/main.cpp src/Node.cpp src/Application.cpp)
target_include_directories(epic_dungeon PUBLIC /include)
target_compile_features(epic_dungeon PRIVATE cxx_std_17)
target_link_libraries(epic_dungeon PRIVATE SFML::Graphics)