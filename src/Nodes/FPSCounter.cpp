#include "Nodes/FPSCounter.hpp"
#include "Nodes/RenderManager.hpp"
#include <iomanip>
#include <sstream>

FPSCounter::FPSCounter()
{
    if(!font.openFromFile("Fonts/ARIAL.TTF"))
    {
        throw "Font not found\n";
    }

    text = std::make_unique<sf::Text>(font);
    /*
        Pretend that im getting a reference to
        a render manager and passing sf::Text to
        UI layer here
    */
}

void FPSCounter::set_position(sf::Vector2f pos)
{
    text->setPosition(pos);    //flipping for more intuitive usage
}

void FPSCounter::update(float delta)
{
    float fps = 1 / delta;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << fps;
    text->setString(ss.str());
}