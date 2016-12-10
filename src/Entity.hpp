#pragma once

#include <Thor/Input.hpp>
#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    virtual ~Entity() = default;

    virtual void update(sf::Time) = 0;
    virtual void connect(thor::ActionMap<std::string>::CallbackSystem&) = 0;
};
