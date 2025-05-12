set(SOURCES
    TimedEvent.cpp
    WindowEventManager.cpp
)

add_library(Events ${SOURCES})

target_include_directories(Nodes PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Events PUBLIC SFML::Graphics Nodes)