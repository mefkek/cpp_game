#include "Nodes/CollisionManager.hpp"
#include "../../include/Ui/MouseCollider.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
// === CollisionManager ===

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
                            a->get_on_collision_event().lock()->rise(b);
                            b->get_on_collision_event().lock()->rise(a);
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
}