#pragma once

#include "Entity.hpp"
#include "Animation.hpp"

#include <Thor/Animations.hpp>

#include <vector>

class PlayerEntity : public Entity, public Animation
{
public:
    PlayerEntity(const sf::Texture& texture);

    void direction(sf::Vector2i direction);

    void update(sf::Time dt) final;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    void connect(thor::ActionMap<std::string>::CallbackSystem& callback_system) final;

private:
    sf::Sprite m_sprite;
    sf::Vector2i m_direction;
    sf::Vector2f m_velocity;
};
