#pragma once
#include "Node.hpp"
#include "RenderManager.hpp"    //tmp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeindex>
#include <map>

class ManagerNotFoundException : public std::exception
{
    private:
    std::string msg;
    public:
    ManagerNotFoundException(std::string type_name)
    {
        msg = "Manager of type: " + type_name +  " haven't been found.\n";
    }
    const char* what() const noexcept override
    {
        return msg.c_str();
    }
};

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
    std::shared_ptr<sf::CircleShape> c_ptr; //tmp
    std::map<std::type_index, std::shared_ptr<Node>> managers;
    Application();

    template <typename T, typename ... Args>
    void register_manager(Args&&... args)
    {
        managers[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
    }
    public:
    Application(const Application&) = delete;   //delete so it can't be copied
    Application& operator=(const Application&) = delete;   //delete so it can't be claimed
    static Application& instance();
    void run();
    sf::RenderWindow& get_window();

    template <typename T>
    std::shared_ptr<T> get_manager()
    {
        if(managers.count(std::type_index(typeid(T))))
        {
            return std::dynamic_pointer_cast<T>(managers[std::type_index(typeid(T))]);
        }

        throw ManagerNotFoundException(std::type_index(typeid(T)).name());
        return nullptr;
    }
    void close();
};