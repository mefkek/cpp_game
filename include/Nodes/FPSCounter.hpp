#pragma once

#include "Node.hpp"
#include "Nodes/Label.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class FPSCounter : public Label
{
private:
    float time_passed;
    unsigned int frames;

public:
    using Label::Label;

    void update(float delta) override;
};