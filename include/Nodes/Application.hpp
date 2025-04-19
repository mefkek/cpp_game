#pragma once
#include "Node.hpp"
#include <SFML/Graphics.hpp>

class Application
{
    /*
        Singleton Application class, should be run as main and then just pretty much left alone.
        Acts as storage and calls all menagers to update game state.

        Not multithread proof

        Could add as tree root
    */
    private:
    sf::RenderWindow window;
    sf::Clock clock;
    std::shared_ptr<Node> root; //temporary
    Application();
    public:
    Application(const Application&) = delete;   //delete so it can't be copied
    Application& operator=(const Application&) = delete;   //delete so it can't be claimed
    static Application& instance();
    void run();
    sf::RenderWindow& get_window();
    void close();
};