file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(Nodes ${SOURCES})

target_include_directories(Nodes PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Nodes PUBLIC SFML::Graphics Ui)