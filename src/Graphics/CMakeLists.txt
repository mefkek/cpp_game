set(SOURCES
        Animation.cpp
        AnimationHelper.cpp
        Background.cpp
)

add_library(Animation ${SOURCES})

target_include_directories(Animation PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/Nodes
)


target_link_libraries(Animation PUBLIC
        SFML::Graphics
        Nodes
)