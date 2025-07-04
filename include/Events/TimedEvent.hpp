#pragma once
#include "EventBase.hpp"

class TimedEvent : public EventBase<>
{
    private:
    const float time_set;
    float time_elapsed;
    int repetitions;

    public:
    TimedEvent(float time, int reps = 1);
    void update(float delta) override;
};