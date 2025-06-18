#pragma once
#include <vector>
#include "Node.hpp"
#include "Colliders.hpp"
#include "TriggerArea.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include <memory>

class CollisionLayer 
{
    public:
    std::string name;
    std::vector<std::weak_ptr<Collider>> colliders;
};

class CollisionManager : public Node
{
    private:
    std::map<unsigned char, CollisionLayer> collision_layers;

    public:
    void collide();
    void add_collider(const std::string& layername, std::weak_ptr<Collider> collider);
    void add_collider(const std::string& layername, std::weak_ptr<TriggerArea> area);
    void add_layer(const std::string& name, unsigned char id);
    void remove_collider(const std::string& layerName, std::weak_ptr<Collider> collider);
    void remove_layer(const std::string& name);
    virtual void update(float delta) override;
};