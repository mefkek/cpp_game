#include "Nodes/FPSCounter.hpp"
#include "Nodes/RenderManager.hpp"
#include "Utility/Exceptions.hpp"
#include <iomanip>
#include <sstream>

FPSCounter::FPSCounter()
{
    if(!font.openFromFile("Fonts/ARIAL.TTF"))
    {
        throw FileNotFoundException("Fonts/ARIAL.TTF");
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
    time_passed += delta;
    frames++;
    if(time_passed == 0)
    {
        return;
    }
    
    int updates_per_second = 5;
    if(time_passed >= (1.f/updates_per_second))
    {
        float fps = frames / time_passed;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << fps;
        text->setString(ss.str());
        time_passed = 0;
        frames = 0;
    }
}