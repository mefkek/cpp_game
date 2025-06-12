#include all .cpp file in current directory
file(GLOB SOURCES
        *.cpp
)

add_library(Ui ${SOURCES})

target_include_directories(Ui PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Ui PUBLIC SFML::Graphics Nodes)