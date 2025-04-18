add_library(Nodes Node.cpp Application.cpp SceneManager.cpp ${PROJECT_SOURCE_DIR}/include/Nodes.hpp)

target_include_directories(Nodes PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Nodes PUBLIC SFML::Graphics)