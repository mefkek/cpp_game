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

class DuplicateRenderLayerException : public std::exception
{
    /*
        if (two layers with the same name) then
            dont
        endif
    */
    private:
    std::string message = "Attempted to create a render layer with duplicate name: ";
    public:
    DuplicateRenderLayerException(const std::string& name)
    {
        message += name + ".\n";
    }
    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class NoWindowGivenException: public std::exception
{
    /*
        for when there is no window given
    */
    public:
    const char* what() const noexcept override
    {
        return "Render manger: no window given.\n";
    }
};

RenderLayer::RenderLayer(const std::string& name, sf::Vector2u size)
{
    this->tex = std::make_unique<sf::RenderTexture>(size);
    sp = std::make_unique<sf::Sprite>(this->tex->getTexture());
    this->name = name;
}

void RenderManager::set_window(sf::RenderWindow* window)
{
    //set window, for scaling
    this->window_ptr = window;
}

sf::Vector2u RenderManager::get_render_texture_size(const std::string& name)
{
    /*
        For placing entities into their respective places
    */
    auto it = std::find_if(layers.begin(), layers.end(),
                            [&](const std::pair<const unsigned char, RenderLayer>& other){return other.second.name == name;});

    if(it != layers.end())
    {
        return it->second.tex->getTexture().getSize();
    }

    std::cerr << "Layer with name: " << name << " has not been found.\n";
    return {-1u, -1u};
}

void RenderManager::add_layer(const std::string& name, char priority, sf::Vector2u size)
{
    //add layers at the correct priority, if layer with the same name exits then delete system32
    if(!window_ptr)
    {
        throw NoWindowGivenException();
    }

    auto it = std::find_if(layers.begin(), layers.end(), [&](const std::pair<const unsigned char, RenderLayer>& other)
                                                {return other.first == priority || other.second.name == name;}
                );

    if(it != layers.end())
    {
        throw DuplicateRenderLayerException(name); 
        //this could be an cerr message, but for safety reson it throw an exception
    }

    layers[priority] = RenderLayer(name, size);
    layers[priority].sp->setScale(
        {
            window_ptr->getSize().x /static_cast<float>(layers[priority].tex->getTexture().getSize().x),
            window_ptr->getSize().y / static_cast<float>(layers[priority].tex->getTexture().getSize().y)
        }
    );
    /*
        If this turns out to be too slow a map or a vector with names could be added as a lookup
    */
}

void RenderManager::add_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw)
{
    auto it = std::find_if(layers.begin(), layers.end(),
                           [&](const std::pair<const unsigned char, RenderLayer>& other)
                           {
                                return other.second.name == layer;
                           });

    if(it != layers.end())
    {
        it->second.drawables.push_back(dw);
        return;
    }

    std::cerr << "Layer with the name: " << layer << " has not been found\n";
}

void RenderManager::remove_drawable(const std::string& layer, const std::weak_ptr<sf::Drawable>& dw)
{
    auto it = std::find_if(layers.begin(), layers.end(),
                           [&](const std::pair<const unsigned char, RenderLayer>& other)
                           {
                                return other.second.name == layer;
                           });

    if(it != layers.end())
    {
        //find sf::Drawable, wont work for nullptr (as it should)
        auto dw_it = std::find_if(it->second.drawables.begin(), it->second.drawables.end(),
                                [&](std::weak_ptr<sf::Drawable> other)
                                    {
                                        return !dw.expired() && !other.expired()
                                                && dw.lock() == other.lock();
                                    }
                                );
            
        if(dw_it != it->second.drawables.end())
        {
            it->second.drawables.erase(dw_it);
            return;
        }
            
        std::cerr << "Drawable with address: " << dw.lock().get() << " has not been found\n";
        //when nulltr prints "Drawable with address: 0x0 has not been found"
    }

    std::cerr << "Layer with the name: " << layer << " has not been found\n";
}

void RenderManager::remove_layer(const std::string& name)
{
    auto it = std::find_if(layers.begin(), layers.end(),
                           [&](const std::pair<const unsigned char, RenderLayer>& other)
                           {
                                return other.second.name == name;
                           });

    if(it != layers.end())
    {
        layers.erase(it);
        return;
    }
    
    std::cerr << "Layer with name: " << name << " has not been found\n";
}

void RenderManager::move_view(const std::string& layer, sf::Vector2f offset)
{
    auto it = std::find_if(layers.begin(), layers.end(),
                           [&](const std::pair<const unsigned char, RenderLayer>& other)
                           {
                                return other.second.name == layer;
                           });

    if(it != layers.end())
    {
        sf::View view = it->second.tex->getDefaultView();
        view.move(-offset);     //flipped for intuitive usage
        it->second.tex->setView(view);
    }
}

void RenderManager::rescale()
{
    if(!window_ptr)
    {
        throw NoWindowGivenException();
    }

    for(auto& [priority, layer] : layers)
    {
        layer.sp->setScale(
            {
                window_ptr->getSize().x /
                static_cast<float>(layer.tex->getTexture().getSize().x),
                window_ptr->getSize().y /
                static_cast<float>(layer.tex->getTexture().getSize().y)
            }
        );
    }
}

void RenderManager::update(float delta)
{
    window_ptr->clear();

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

        window_ptr->draw(*(layer.sp));
    }

    window_ptr->display();
}
