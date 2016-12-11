#include "PlayerEntity.hpp"
#include <iostream>

PlayerEntity::PlayerEntity(const sf::Texture& texture)
: Animation(std::string("entities/player.json"))
, m_sprite{texture}
, m_velocity{200.f, 200.f}
{
}

void PlayerEntity::direction(sf::Vector2i direction)
{
    // As previously set.
    if(direction.x == 2)
        direction.x = m_direction.x;
    if(direction.y == 2)
        direction.y = m_direction.y;

    m_direction = direction;
}

sf::FloatRect PlayerEntity::getGlobalBounds()
{
    return {getPosition(), {m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height}};
}

void PlayerEntity::update(sf::Time dt)
{
    m_animator.update(dt);

    move(sf::Vector2f{m_velocity.x * m_direction.x, m_velocity.y * m_direction.y} * dt.asSeconds());
}

void PlayerEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}

void PlayerEntity::connect(thor::ActionMap<std::string>::CallbackSystem& callback_system)
{
    callback_system.connect("left", std::bind(&PlayerEntity::direction, this, sf::Vector2i{-1, 2}));
    callback_system.connect("left_released", std::bind(&PlayerEntity::direction, this, sf::Vector2i{0, 2}));
    callback_system.connect("right", std::bind(&PlayerEntity::direction, this, sf::Vector2i{1, 2}));
    callback_system.connect("right_released", std::bind(&PlayerEntity::direction, this, sf::Vector2i{0, 2}));
    callback_system.connect("up", std::bind(&PlayerEntity::direction, this, sf::Vector2i{2, -1}));
    callback_system.connect("up_released", std::bind(&PlayerEntity::direction, this, sf::Vector2i{2, 0}));
    callback_system.connect("down", std::bind(&PlayerEntity::direction, this, sf::Vector2i{2, 1}));
    callback_system.connect("down_released", std::bind(&PlayerEntity::direction, this, sf::Vector2i{2, 0}));
}
