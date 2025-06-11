#include all .cpp file in current directory
file(GLOB SOURCES
        *.cpp
)

add_library(Nodes ${SOURCES})

target_include_directories(Nodes PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Nodes PUBLIC SFML::Graphics Ui)