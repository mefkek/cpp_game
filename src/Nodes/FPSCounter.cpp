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
    /*
        Pretend that im getting a reference to
        a render manager and passing sf::Text to
        UI layer here
    */
}

void FPSCounter::initialize()
{
    label = add_child<Label>("Debug_ui", font).lock();
}

void FPSCounter::set_position(sf::Vector2f pos)
{
    label->set_position(pos);
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
        label->set_string(ss.str());
        time_passed = 0;
        frames = 0;
    }
}