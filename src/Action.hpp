#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Input.hpp>

class Action
{
public:
    Action(const std::string& name, sf::IntRect rect);

    std::string name() const;
    bool check(sf::Vector2i position) const;

private:
    std::string m_name;
    sf::IntRect m_rect;
};
