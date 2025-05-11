#include "Nodes/EventManager.hpp"

TimedEvent::TimedEvent(float time, int reps) : time_set(time), repetitions(reps) {}

void TimedEvent::update(float delta)
{
    time_elapsed += delta;
    if(time_elapsed >= time_set)
    {
        this->rise();
        time_elapsed = 0.f;
        if(repetitions < 0)
        {
            --repetitions;
        }
    }

    if(repetitions == 0)
    {
        //events that sould happen infite or unspecified amount of times
        //will have repetitions set to negative        this->kill();
    }
}