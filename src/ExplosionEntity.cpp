#include "ExplosionEntity.hpp"

ExplosionEntity::ExplosionEntity()
: Animation(std::string("test.json"))
{
    m_animator.play() << "test" << thor::Playback::loop("test");
}

void ExplosionEntity::texture(const sf::Texture& texture)
{
    m_sprite.setTexture(texture, true);
}

void ExplosionEntity::update(sf::Time dt)
{
    m_animator.update(dt);
    m_animator.animate(m_sprite);
}

void ExplosionEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //states.transform *= getTransform();
    target.draw(m_sprite, states);
}
