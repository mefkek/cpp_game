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

RenderLayer::RenderLayer(std::string name) : name(name) {};

Render::Render(sf::RenderTexture n_tex)
{
    tex = std::make_unique<sf::RenderTexture>(std::move(n_tex));
    sp = std::make_unique<sf::Sprite>(tex->getTexture());
};

void RenderManager::set_window(sf::RenderWindow* window)
{
    //set window, for scaling
    this->window_ptr = window;
}

sf::Vector2u RenderManager::get_render_texture(std::string name)
{
    /*
        For placing entities into their respective places
    */
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
            return renders[priority].tex->getSize();
        }
    }

    std::cerr << "Layer with name: " << name << " has not been found.\n";
    return {-1u, -1u};
}

void RenderManager::add_layer(std::string name, char priority, sf::Vector2u size)
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
            throw DuplicateRenderLayerException(name);
        }
    }

    layers[priority].emplace_back<RenderLayer>(name);
    if(!renders.count(priority))     //for map 0 or 1
    {
        renders[priority] = Render(sf::RenderTexture(size));  //640 x 360 by default
        sf::Vector2f scale(
            {
                window_ptr->getSize().x /static_cast<float>(renders[priority].tex->getTexture().getSize().x),
                window_ptr->getSize().y / static_cast<float>(renders[priority].tex->getTexture().getSize().y)
            });
        renders[priority].sp->setScale(scale);
        renders[priority].sp->setPosition({0.f, 0.f});
    }

    /*
        If this turns out to be too slow a map or a vector with names could be added as a lookup
    */
}

void RenderManager::add_drawable(std::string layer, const std::weak_ptr<sf::Drawable>& dw)
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

void RenderManager::remove_drawable(std::string layer, const std::weak_ptr<sf::Drawable>& dw)
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
            if(layers_v.empty())
            {
                renders.erase(priority);
            }
            return;
        }
    }
    
    std::cerr << "Layer with name: " << name << " has not been found\n";
}

void RenderManager::move_view(std::string layer_name, sf::Vector2f offset)
{
    for(auto& [priority, layers_v] : layers)
    {
        auto it = std::find_if(layers_v.begin(), layers_v.end(),
                        [&](const RenderLayer& a)
                        {
                            return a.name == layer_name;
                        }
                    );
        if(it != layers_v.end())
        {
            sf::View view = renders[priority].tex->getDefaultView();
            view.move(offset);
            renders[priority].tex->setView(view);
        }
    }
}

void RenderManager::update(float delta)
{
    //remove nullptr and draw stuff to render textures
    for(auto& [priority, layers_v] : layers)
    {
        renders[priority].tex->clear();

        for(auto& layer : layers_v)
        {
            layer.drawables.erase(std::remove_if(layer.drawables.begin(), layer.drawables.end(),
                                        [](std::weak_ptr<sf::Drawable>& a) {return a.expired();}),
                                  layer.drawables.end());

            for(auto& dw_ptr : layer.drawables)
            {
                if(auto p = dw_ptr.lock())
                {
                    renders[priority].tex->draw(*p);
                }
            }
        }

        renders[priority].tex->display();
    }

    window_ptr->clear();

    for(auto& [priority, render] : renders)
    {
        window_ptr->draw(*(render.sp));
    }

    //diplay
    window_ptr->display();
}
