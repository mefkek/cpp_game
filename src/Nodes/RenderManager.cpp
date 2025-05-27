#include <iostream>
#include <algorithm>
#include "Nodes/Application.hpp"
#include "Nodes/RenderManager.hpp"
#include "Utility/Exceptions.hpp"

/*
    NOTES:

    Technically speaking layers should only be added either on
    application startup, maybe on scene change, so maybe befriending
    related construction should be included. For now its good.
*/

RenderLayer::RenderLayer(sf::Vector2u size)
{
    this->tex = std::make_unique<sf::RenderTexture>(size);
    sp = std::make_unique<sf::Sprite>(this->tex->getTexture());
}

sf::Vector2u RenderManager::get_render_texture_size(const std::string& name)
{
    /*
        For placing entities into their respective places
    */
    

    if(string_ref.count(name))
    {
        if(layers.count(string_ref[name]))
        {
            return layers[string_ref[name]].tex->getTexture().getSize();
        }
    }

    Logger::log(Logger::MessageType::Warning, "Layer with name: ", name, " has not been found.");
    return {-1u, -1u};
}

void RenderManager::add_layer(const std::string& name, unsigned char priority, sf::Vector2u size)
{
    if(layers.count(priority) || string_ref.count(name))
    {
        throw DuplicateRenderLayerException(name);
    }

    string_ref[name] = priority;
    layers[priority] = RenderLayer(size);
    layers[priority].sp->setScale(
        {
            Application::instance().get_window().getSize().x 
                /static_cast<float>(layers[priority].tex->getTexture().getSize().x),
            Application::instance().get_window().getSize().y / 
                static_cast<float>(layers[priority].tex->getTexture().getSize().y)
        }
    );
}

void RenderManager::add_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw)
{
    if(string_ref.count(layer))
    {
        layers[string_ref[layer]].drawables.push_back(dw);
        return;
    }

    Logger::log(Logger::MessageType::Warning, "Layer with name: ", layer, " has not been found.");
}

void RenderManager::remove_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw)
{
    if(string_ref.count(layer))
    {
        unsigned char p = string_ref[layer];
         //find sf::Drawable, wont work for nullptr (as it should)
        auto dw_it = std::find_if(layers[p].drawables.begin(), layers[p].drawables.end(),
        [&](std::weak_ptr<sf::Drawable> other)
            {
                return !dw.expired() && !other.expired()
                        && dw.lock() == other.lock();
            }
        );

        if(dw_it != layers[p].drawables.end())
        {
           layers[p].drawables.erase(dw_it);
            return;
        }

        Logger::log(Logger::MessageType::Warning, "Drawable with address: ", dw.lock().get(), " has not been found.");
        //when nulltr prints "Drawable with address: 0x0 has not been found"
    }

    Logger::log(Logger::MessageType::Warning, "Layer with name: ", layer, " has not been found.");
}

void RenderManager::remove_layer(const std::string& name)
{
    if(string_ref.count(name))
    {
        auto it = std::find_if(layers.begin(), layers.end(),
        [&](const std::pair<const unsigned char, RenderLayer>& other)
        {
             return other.first == string_ref[name];
        });

        if(it != layers.end())
        {
            layers.erase(it);
            return;
        }
    }
    
    Logger::log(Logger::MessageType::Warning, "Layer with name: ", name, " has not been found.");
}

void RenderManager::move_view(const std::string& layer, sf::Vector2f offset)
{
    if(string_ref.count(layer))
    {
        sf::View view = layers[string_ref[layer]].tex->getDefaultView();
        view.move(-offset);     //flipped for intuitive usage
        layers[string_ref[layer]].tex->setView(view);
        return;
    }

    Logger::log(Logger::MessageType::Warning, "Layer with name: ", layer, " has not been found.");
}

void RenderManager::rescale()
{
    for(auto& [priority, layer] : layers)
    {
        layer.sp->setScale(
            {
                Application::instance().get_window().getSize().x /
                static_cast<float>(layer.tex->getTexture().getSize().x),
                Application::instance().get_window().getSize().y /
                static_cast<float>(layer.tex->getTexture().getSize().y)
            }
        );
    }
}

void RenderManager::update(float delta)
{
    Application::instance().get_window().clear();

    //remove nullptr and draw stuff to render textures
    for(auto& [priority, layer] : layers)
    {
        layer.tex->clear(sf::Color::Transparent);

        layer.drawables.erase(std::remove_if(layer.drawables.begin(), layer.drawables.end(),
                                    [](std::weak_ptr<sf::Drawable>& a) {return a.expired();}),
                                layer.drawables.end());
            
        for(auto& dw_ptr : layer.drawables)
        {
            if(auto p = dw_ptr.lock())
            {
                layer.tex->draw(*p);
            }
        }

        layer.tex->display();

        Application::instance().get_window().draw(*(layer.sp));
    }

    Application::instance().get_window().display();
}
