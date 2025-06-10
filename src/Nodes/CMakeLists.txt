set(SOURCES
        Node.cpp
        Application.cpp
        RenderManager.cpp
        FPSCounter.cpp
        TextureAtlas.cpp
        Colliders.cpp
        CollisionManager.cpp
        TriggerArea.cpp
        ActorManager.cpp
        Actor.cpp
        Party.cpp
)

add_library(Nodes ${SOURCES})

target_include_directories(Nodes PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Nodes PUBLIC SFML::Graphics)