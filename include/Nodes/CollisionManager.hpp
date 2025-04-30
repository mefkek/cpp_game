#pragma once
#include <vector>
#include "Nodes/Node.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <map>
#include <memory>

class Collider {
public:
  virtual void on_collision() = 0;
  virtual ~Collider() = default;
};

class CollisionLayer {
public:
  std::string name;
  std::vector<std::weak_ptr<Collider>> colliders;
};

class CollisionManager : public Node{
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

class TriggerArea : public Collider {
protected:
  bool triggered = false;

public:
  void on_collision() override;
  virtual void on_entered() = 0;
  virtual void on_exit() = 0;
virtual ~TriggerArea() = default;
};

class Button : public Collider {
private:
  sf::Text text;
  std::function<void()> callback;

public:
  Button(const std::string& label, const sf::Font& font, std::function<void()> cb);

  void on_collision() override;
  void set_text(const std::string& new_text);
  sf::Text& get_text();
};