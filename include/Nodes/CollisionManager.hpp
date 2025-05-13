#pragma once
#include <vector>
#include "Node.hpp"
#include "Colliders.hpp"
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

/*
//this should be somewhere else
class Button : public Collider 
{
  private:
  sf::Text text;
  std::function<void()> callback;

  public:
  Button(const std::string& label, const sf::Font& font, std::function<void()> cb);

  bool collides_with(const std::shared_ptr<Collider> other) const override;
  void set_text(const std::string& new_text);
  sf::Text& get_text();
};
*/

//debug
#include "Nodes.hpp"

class DebugRect : public Node
{
    private:
    friend class DebugTrigger;
    std::shared_ptr<TriggerArea> trigger;
    std::shared_ptr<sf::RectangleShape> shape;

    public:
    DebugRect() {}
    void update(float delta) {}
    void initialize() override;
};

class DebugCirc : public Node
{
    private:
    friend class DebugTrigger;
    friend class DebugTriggerCirc;

    std::shared_ptr<TriggerArea> trigger;
    std::shared_ptr<sf::CircleShape> shape;

    public:
    DebugCirc(){}
    void update(float delta) override;
    void initialize() override;
};

class DebugTrigger : public TriggerArea
{
    public:
    DebugTrigger(sf::Vector2f pos, sf::Vector2f size) : TriggerArea(pos, size) {}
    void on_entered(const std::weak_ptr<Collider> other) override;
    void on_exit(const std::weak_ptr<Collider> other) override;
};

class DebugTriggerCirc : public TriggerArea
{
    public:
    DebugTriggerCirc(sf::Vector2f pos, float size) : TriggerArea(pos, size) {}
    void on_entered(const std::weak_ptr<Collider> other) override;
    void on_exit(const std::weak_ptr<Collider> other) override;
};