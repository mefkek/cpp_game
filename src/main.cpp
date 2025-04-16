#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    sf::CircleShape c(75);
    c.setFillColor(sf::Color::Green);
    c.setOutlineThickness(-5);
    c.setOutlineColor(sf::Color::Blue);
    c.setOrigin({75.f, 75.f});
    c.setPosition({1920/2.f, 1080/2.f});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.draw(c);
        window.display();
    }
}
