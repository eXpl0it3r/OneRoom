#include "Screen.hpp"

Screen::Screen(const std::string& tag, const std::string& name, const sf::Texture& texture, std::vector<Action>&& actions)
: m_tag{tag}
, m_name{name}
, m_sprite{texture}
, m_actions{std::forward<std::vector<Action>>(actions)}
{
}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}

void Screen::on_click(sf::Vector2i position)
{
    m_action = "";

    for(auto& action : m_actions)
    {
        if(action.check(position))
        {
            m_action = action.name();
        }
    }
}

void Screen::on_hover(sf::Vector2i position)
{
    m_hovering = "";

    for(auto& action : m_actions)
    {
        if(action.check(position))
        {
            m_hovering = action.name();
        }
    }
}

std::string Screen::tag() const
{
    return m_tag;
}

std::string Screen::name() const
{
    return m_name;
}

std::string Screen::action()
{
    std::string action = m_action;
    m_action = "";
    return action;
}

std::string Screen::hovering()
{
    return m_hovering;
}
