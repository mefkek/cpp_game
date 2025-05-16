set(SOURCES
    DungeonManager.cpp
    ChunkGenerator.cpp
    Room.cpp
)

add_library(DungeonManager ${SOURCES})

target_include_directories(DungeonManager PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(DungeonManager PUBLIC SFML::Graphics Nodes)