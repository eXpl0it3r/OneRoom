#include "Tile.hpp"

Tile::Tile(const sf::Texture& texture, const sf::IntRect& rect)
: m_sprite{texture, rect}
{
}

void Tile::update(sf::Time dt)
{
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}

void Tile::connect(thor::ActionMap<std::string>::CallbackSystem&)
{

}