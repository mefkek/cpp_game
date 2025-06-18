set(SOURCES
        Animation.cpp
        AnimationHelper.cpp
        Background.cpp
)

add_library(Graphics ${SOURCES})

target_include_directories(Graphics PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/Nodes
)


target_link_libraries(Graphics PUBLIC
        SFML::Graphics
        Nodes
)