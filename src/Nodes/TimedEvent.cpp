#include "Nodes/EventManager.hpp"

TimedEvent::TimedEvent(float time, std::function<void(void)> callable, std::weak_ptr<Node> caller, int reps) :
                       time_set(time), callable(callable), caller(caller), repetitions(reps) {}

void TimedEvent::update(float delta)
{
    if(caller.lock())
    {
        time_elapsed += delta;
        if(time_elapsed >= time_set)
        {
            time_elapsed = 0.f;
            if(repetitions < 0)
            {
                --repetitions;
            }
            callable();
        }

        if(repetitions == 0)
        {
            //events that sould happen infite or unspecified amount of times
            //will have repetitions set to negative
            this->kill();
        }
    }
    else
    {
        this->kill();
    }
}