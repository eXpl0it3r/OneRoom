#include "Action.hpp"

Action::Action(const std::string& name, sf::IntRect rect)
: m_name{name}
, m_rect{rect}
{
}

std::string Action::name() const
{
    return m_name;
}

bool Action::check(sf::Vector2i position) const
{
    return m_rect.contains(position);
}
