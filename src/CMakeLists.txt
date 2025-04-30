add_subdirectory(Nodes)

add_executable(epic_dungeon main.cpp ${PROJECT_SOURCE_DIR}/include/Nodes.hpp)
target_include_directories(epic_dungeon PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(epic_dungeon PUBLIC SFML::Graphics Nodes)
