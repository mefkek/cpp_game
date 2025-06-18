#include "Graphics/Background.hpp"
#include "Nodes/RenderManager.hpp"
#include <stdexcept>

void Background::initialize()
{
    auto manager = Application::instance().get_manager<RenderManager>();
    manager->add_drawable("Background", std::dynamic_pointer_cast<sf::Drawable>(shared_from_this()));

    auto layer_size = manager->get_render_texture("Background").getSize();
    setScale({
        layer_size.x / static_cast<float>(getTexture().getSize().x),
        layer_size.y / static_cast<float>(getTexture().getSize().y)
    });
}

void Background::update(float delta) {}