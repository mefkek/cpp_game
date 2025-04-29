#pragma once
#include "Utility/Exceptions.hpp"
#include "Utility/Various.hpp"
#include "Nodes.hpp"
#include <SFML/Graphics.hpp>
#include <typeindex>
#include <iostream>
#include <mutex>
#include <map>

class Application
{
    /*
        Singleton Application class, should be run as main and then just pretty much left alone.
        Acts as storage and calls all menagers to update game state.

        Not multithread proof

        Could act as tree root
    */
    private:
    sf::RenderWindow window;
    sf::Clock clock;
    std::shared_ptr<Node> root; //temporary (?)
    std::map<std::type_index, std::shared_ptr<Node>> managers;
    static std::mutex application_mutex;

    Application();

    public:
    Application(const Application&) = delete;   //delete so it can't be copied

    Application& operator=(const Application&) = delete;   //delete so it can't be claimed

    static Application& instance();

    void run();

    sf::RenderWindow& get_window();
    
    template <typename T, typename ... Args>
    void register_manager(Args&&... args)
    {
        managers[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    std::shared_ptr<T> get_manager()
    {
        if(managers.count(std::type_index(typeid(T))))
        {
            return std::dynamic_pointer_cast<T>(managers[std::type_index(typeid(T))]);
        }

        #ifdef __GNUG__ //reference Node.cpp for reference
        throw ManagerNotFoundException(demangle(std::type_index(typeid(T)).name()));
        #else
        throw ManagerNotFoundException(std::type_index(typeid(T)).name());
        #endif

        return nullptr;
    }

    void close();
};