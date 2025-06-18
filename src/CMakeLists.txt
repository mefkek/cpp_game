add_subdirectory(Nodes)
add_subdirectory(Utility)
add_subdirectory(Events)
add_subdirectory(Tilemap)
add_subdirectory(DungeonManager)
add_subdirectory(Ui)
add_subdirectory(GameStateManager)

add_executable(epic_dungeon main.cpp
        Combat/CombatManager.cpp
        ../include/Combat/EnemyFactory.hpp)
target_include_directories(epic_dungeon PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(epic_dungeon PUBLIC SFML::Graphics Nodes Events DungeonManager Utility Tilemap Ui GameStates)