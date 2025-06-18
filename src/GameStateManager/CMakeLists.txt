#include all .cpp file in current directory
file(GLOB SOURCES
        *.cpp
)

add_library(GameStates ${SOURCES}
        CombatState.cpp)

target_include_directories(GameStates PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(GameStates PUBLIC SFML::Graphics Nodes DungeonManager)