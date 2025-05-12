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
  sf::Vector2f position;
  sf::Vector2f size;

  virtual ~Collider() = default;

  virtual bool collides_with(const Collider& other) const = 0;
  virtual void on_collision() = 0;
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
  bool collides_with(const Collider& other) const override;
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

  bool collides_with(const Collider& other) const override;
  void on_collision() override;
  void set_text(const std::string& new_text);
  sf::Text& get_text();
};

class RectangleCollider : public Collider {
public:
  RectangleCollider(const sf::Vector2f& pos, const sf::Vector2f& size_) {
    position = pos;
    size = size_;
  }
  bool collides_with(const Collider& other) const override
{
  return !(position.x + size.x < other.position.x || position.x > other.position.x + other.size.x || position.y + size.y < other.position.y || position.y > other.position.y + other.size.y);
}
  void on_collision() override {
  std::cout << "O jezus maria zdażyła się awaria! Rectangle collided \n";
}
};

class CircleCollider : public Collider {
  public:
  float radius;

  CircleCollider(const sf::Vector2f& center, float r) : radius(r) {
    position = center;
    size = sf::Vector2f(radius*2,radius*2);
  }
  bool collides_with(const Collider& other) const override {
    const CircleCollider* circle = dynamic_cast<const CircleCollider*>(&other);
    if (circle) {
      float dx = position.x - circle->position.x;
      float dy = position.y - circle->position.y;
      float dist_sq = dx * dx + dy * dy;
      float radius_sum = radius + circle->radius;
      return dist_sq <= radius_sum * radius_sum;
    }

    sf::Vector2f rect_pos = other.position;
    sf::Vector2f rect_size = other.size;
    float closestX = std::clamp(position.x, rect_pos.x, rect_pos.x + rect_size.x);
    float closestY = std::clamp(position.y, rect_pos.y, rect_pos.y + rect_size.y);

    float dx = position.x - closestX;
    float dy = position.y - closestY;
    return (dx * dx + dy * dy) <= (radius * radius);
  }
  void on_collision() override {
    std::cout << "O jezus maria zdażyła się awaria Circle collided \n";
  }
};