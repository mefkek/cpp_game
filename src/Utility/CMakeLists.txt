set(SOURCES
    Logger.cpp
    Exceptions.cpp
    Various.cpp
)

add_library(Utility ${SOURCES})

target_include_directories(Utility PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Utility PUBLIC SFML::Graphics Nodes)