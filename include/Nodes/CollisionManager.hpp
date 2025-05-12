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