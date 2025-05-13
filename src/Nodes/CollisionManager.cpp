#include "Nodes/CollisionManager.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
// === CollisionManager ===

/// Perform collision detection between all colliders.
/// This is currently just a placeholder.
void CollisionManager::collide()
{
    for (auto& [id, layer] : collision_layers)
    {
        auto& colliders = layer.colliders;

        colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
            [](const std::weak_ptr<Collider>& c)
            {
                return c.expired();
            }), colliders.end());

        for (size_t i = 0; i < colliders.size(); ++i)
        {
            if(auto a = colliders[i].lock())
            {
                for (size_t j = i + 1; j < colliders.size(); ++j)
                {
                    if(auto b = colliders[j].lock())
                    {                    
                        if (a->collides_with(b)) 
                        {
                            a->on_collision(b);
                            b->on_collision(a);
                        }
                    }
                }
            }
        }
    }
}

/// Add a collider to a specific collision layer by name.
void CollisionManager::add_collider(const std::string& layername, std::weak_ptr<Collider> collider)
{
    for (auto& [id, layer] : collision_layers)
    {
        if (layer.name == layername) {
            layer.colliders.push_back(collider);
            return;
        }
    }
}

void CollisionManager::add_collider(const std::string& layername, std::weak_ptr<TriggerArea> area)
{
    if(auto area_ptr = area.lock())
    {
        for (auto& [id, layer] : collision_layers)
        {
            if (layer.name == layername){
                layer.colliders.push_back(area_ptr->collider);
                return;
            }
        }
    }
}

/// Add a new collision layer with a given name and ID.
void CollisionManager::add_layer(const std::string& name, unsigned char id)
{
    CollisionLayer layer;
    layer.name = name;
    collision_layers[id] = layer;
}

/// Remove a collider from a specific layer by name.
void CollisionManager::remove_collider(const std::string& layerName, std::weak_ptr<Collider> collider)
{
    for (auto& [id, layer] : collision_layers) {
        if (layer.name == layerName) {
            auto& vec = layer.colliders;
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                [&](const std::weak_ptr<Collider>& col) {
                    return col.lock() == collider.lock();  // Compare shared_ptr contents
                }), vec.end());
            return;
        }
    }
}

/// Remove a collision layer by name.
void CollisionManager::remove_layer(const std::string& name)
{
    for (auto it = collision_layers.begin(); it != collision_layers.end(); ++it)
    {
        if (it->second.name == name)
        {
            collision_layers.erase(it);
            return;
        }
    }
}

/// Update collision system — usually called once per frame.
void CollisionManager::update(float delta)
{
    collide();  // Call collision detection
    // Other logic (e.g., debug visualization) could be added here
}

/*
// === Button ===

/// Constructor for a Button that displays a label and calls a callback on collision.
Button::Button(const std::string& label, const sf::Font& font, std::function<void()> cb)
    : text(font, label), callback(cb) {}

/// Called when something collides with the button — invokes the callback.
bool Button::collides_with(const std::shared_ptr<Collider> other) const
{
    return !(position.x + size.x < other->position.x ||
             position.x > other->position.x + other->size.x ||
             position.y + size.y < other->position.y ||
             position.y > other->position.y + other->size.y);
}


/// Change the button's text label.
void Button::set_text(const std::string& new_text)
{
    text.setString(new_text);
}

/// Get a reference to the sf::Text object for rendering.
sf::Text& Button::get_text()
{
    return text;
};
*/

//debug
void DebugRect::initialize()
{
    sf::Vector2f pos;
    pos.x = Application::instance().get_manager<RenderManager>()->
            get_render_texture("Debug_ui").getSize().x / 2.f;
    pos.y = Application::instance().get_manager<RenderManager>()->
            get_render_texture("Debug_ui").getSize().y / 2.f;
    shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(250.f, 250.f));
    shape->setOrigin({250.f / 2, 250.f / 2});
    shape->setPosition(pos);
    trigger = add_child<DebugTrigger>(pos, sf::Vector2f(250.f, 250.f)).lock();
    Application::instance().get_manager<RenderManager>()->add_drawable("Debug_ui", shape);
    Application::instance().get_manager<CollisionManager>()->add_collider("Debug_coll", trigger);
}

void DebugCirc::update(float delta)
{
    //map position to layer coordinates, should propably be done by layer manager
    //resize doesn't work
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(Application::instance().get_window());
    sf::Vector2f mouse_translated = Application::instance().get_window().mapPixelToCoords(mouse_pos);
    sf::Vector2f scale = Application::instance().get_manager<RenderManager>()->
                         get_render_sprite("Debug_ui").getScale();
    mouse_translated.x /= scale.x;
    mouse_translated.y /= scale.y;
    shape->setPosition(mouse_translated);
    trigger->position = shape->getPosition();
}

void DebugCirc::initialize()
{
    sf::Vector2f pos;
    pos.x = Application::instance().get_window().getSize().x / 2;
    pos.y = Application::instance().get_window().getSize().y / 2;
    shape = std::make_shared<sf::CircleShape>(50.f);
    shape->setOrigin({25, 25});
    shape->setPosition(pos);
    trigger = add_child<DebugTriggerCirc>(sf::Vector2f(50.f, 50.f), 50.f).lock();
    Application::instance().get_manager<RenderManager>()->add_drawable("Debug_ui", shape);
    Application::instance().get_manager<CollisionManager>()->add_collider("Debug_coll", trigger);
}

void DebugTrigger::on_entered(const std::weak_ptr<Collider> other)
{
    Logger::log(Logger::MessageType::Info, "DebugRect: ", shared_from_this(), " entered");
    if(auto p = std::dynamic_pointer_cast<DebugRect>(parent.lock()))
    {
        p->shape->setFillColor(sf::Color::Red);
    }
}

void DebugTrigger::on_exit(const std::weak_ptr<Collider> other)
{
    Logger::log(Logger::MessageType::Info, "DebugRect: ", shared_from_this(), " exited");
    if(auto p = std::dynamic_pointer_cast<DebugRect>(parent.lock()))
    {
        p->shape->setFillColor(sf::Color::Blue);
    }
}

void DebugTriggerCirc::on_entered(const std::weak_ptr<Collider> other)
{
    Logger::log(Logger::MessageType::Info, "MouseDebugRect: ", shared_from_this(), " entered");
    if(auto p = std::dynamic_pointer_cast<DebugCirc>(parent.lock()))
    {
        p->shape->setFillColor(sf::Color::Red);
    }
}

void DebugTriggerCirc::on_exit(const std::weak_ptr<Collider> other)
    {
    Logger::log(Logger::MessageType::Info, "MouseDebugRect: ", shared_from_this(), " exited");
    if(auto p = std::dynamic_pointer_cast<DebugCirc>(parent.lock()))
    {
        p->shape->setFillColor(sf::Color::Blue);
    }
}