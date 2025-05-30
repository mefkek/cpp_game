set(SOURCES
    Tilemap.cpp
)

add_library(Tilemap ${SOURCES})

target_include_directories(Tilemap PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Tilemap PUBLIC SFML::Graphics Nodes Utility)