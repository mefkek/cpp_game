#include "Nodes/CollisionManager.hpp"
#include <algorithm>

// === CollisionManager ===

/// Perform collision detection between all colliders.
/// This is currently just a placeholder.
void CollisionManager::collide() {
    // TODO: Add actual collision detection logic here
}

/// Add a collider to a specific collision layer by name.
void CollisionManager::add_collider(const std::string& layername, std::weak_ptr<Collider> collider) {
    for (auto& [id, layer] : collision_layers) {
        if (layer.name == layername) {
            layer.colliders.push_back(collider);
            return;
        }
    }
}

/// Add a new collision layer with a given name and ID.
void CollisionManager::add_layer(const std::string& name, unsigned char id) {
    CollisionLayer layer;
    layer.name = name;
    collision_layers[id] = layer;
}

/// Remove a collider from a specific layer by name.
void CollisionManager::remove_collider(const std::string& layerName, std::weak_ptr<Collider> collider) {
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
void CollisionManager::remove_layer(const std::string& name) {
    for (auto it = collision_layers.begin(); it != collision_layers.end(); ++it) {
        if (it->second.name == name) {
            collision_layers.erase(it);
            return;
        }
    }
}

/// Update collision system — usually called once per frame.
void CollisionManager::update(float delta) {
    collide();  // Call collision detection
    // Other logic (e.g., debug visualization) could be added here
}

// === TriggerArea ===

/// A specialized Collider that triggers an event on first collision.
void TriggerArea::on_collision() {
    if (!triggered) {
        triggered = true;
        on_entered();  // Call user-defined behavior
    }
}

// === Button ===

/// Constructor for a Button that displays a label and calls a callback on collision.
Button::Button(const std::string& label, const sf::Font& font, std::function<void()> cb)
    : text(font, label), callback(cb) {
}

/// Called when something collides with the button — invokes the callback.
void Button::on_collision() {
    if (callback) callback();
}

/// Change the button's text label.
void Button::set_text(const std::string& new_text) {
    text.setString(new_text);
}

/// Get a reference to the sf::Text object for rendering.
sf::Text& Button::get_text() {
    return text;
}