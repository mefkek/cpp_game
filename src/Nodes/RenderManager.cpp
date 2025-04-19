#include <iostream>
#include <algorithm>
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"

/*
    NOTES:

    Technically speaking layers should only be added either on
    application startup, maybe on scene change, so maybe befriending
    related construction should be included. For now its good.
*/

class DuplacateRenderLayerException : public std::exception
{
    /*
        if (two layers with the same name) then
            dont
        endif
    */
    private:
    std::string message = "Attempted to create a render layer with duplicate name: ";
    public:
    DuplacateRenderLayerException(std::string name)
    {
        message += name + ".\n";
    }
    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

RenderLayer::RenderLayer(std::string name) : name(name) {};

void RenderManager::add_layer(std::string name, char priority)
{
    //add layers at the correct priority, if layer with the same name exits then delete system32
    for(auto& [priority, layers_v] : layers)
    {
        auto it = std::find_if(layers_v.begin(), layers_v.end(),
                        [&](const RenderLayer& a)
                        {
                            return a.name == name;
                        }
                    );
        if(it != layers_v.end())
        {
            throw DuplacateRenderLayerException(name);
        }
    }

    layers[priority].emplace_back<RenderLayer>(name);
}

void RenderManager::add_drawable(std::string layer, std::weak_ptr<sf::Drawable> dw)
{
    for(auto& [priority, layers_v] : layers)
    {
        auto it = std::find_if(layers_v.begin(), layers_v.end(),
                            [&](const RenderLayer& a)
                            {
                                return a.name == layer;
                            }
                        );

        if(it != layers_v.end())
        {
            it->drawables.push_back(dw);
            return;
        }
    }

    std::cerr << "Layer with the name: " << layer << " has not been found\n";
}

void RenderManager::remove_drawable(std::string layer, std::weak_ptr<sf::Drawable> dw)
{
    for(auto& [priority, layers_v] : layers)
    {
        //find layer
        auto it = find_if(layers_v.begin(), layers_v.end(),
                            [&](const RenderLayer& a)
                            {
                                return a.name == layer;
                            }
                        );

        if(it != layers_v.end())
        {
            //find sf::Drawable, wont work for nullptr (as it should)
            auto dw_it = std::find_if(it->drawables.begin(), it->drawables.end(),
                                    [&](std::weak_ptr<sf::Drawable> other)
                                    {
                                        return !dw.expired() && !other.expired()
                                                && dw.lock() == other.lock();
                                    }
                                );
            
            if(dw_it != it->drawables.end())
            {
                it->drawables.erase(dw_it);
                return;
            }
            
            std::cerr << "Drawable with address: " << dw.lock().get() << " has not been found\n";
            //when nulltr prints "Drawable with address: 0x0 has not been found"
        }
    }

    std::cerr << "Layer with the name: " << layer << " has not been found\n";
}

void RenderManager::remove_layer(std::string name)
{
    for(auto& [priority, layers_v] : layers)
    {
        //find layer with the same name
        auto it = std::find_if(layers_v.begin(), layers_v.end(),
                            [&](const RenderLayer& a)
                            {
                                return a.name == name;
                            }
                        );

        if(it != layers_v.end())
        {
            layers_v.erase(it);
            return;
        }
    }
    
    std::cerr << "Layer with name" << name << " has not been found\n";
}

void RenderManager::update(float delta)
{
    //clear window
    Application::instance().get_window().clear();

    //remove nullptr and draw stuff
    for(auto& [priority, layers_v] : layers)
    {
        for(auto& layer : layers_v)
        {
            layer.drawables.erase(std::remove_if(layer.drawables.begin(), layer.drawables.end(),
                                        [](std::weak_ptr<sf::Drawable>& a) {return a.expired();}),
                                  layer.drawables.end());

            for(auto& dw_ptr : layer.drawables)
            {
                if(auto p = dw_ptr.lock())
                {
                    Application::instance().get_window().draw(*p);
                }
            }
        }
    }

    //diplay
    Application::instance().get_window().display();
}
