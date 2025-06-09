#include "Nodes/FPSCounter.hpp"
#include "Nodes/RenderManager.hpp"
#include "Utility/Exceptions.hpp"
#include <iomanip>
#include <sstream>

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
        setString(ss.str());
        time_passed = 0;
        frames = 0;
    }
}