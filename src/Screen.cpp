#include "Screen.hpp"

Screen::Screen(const std::string& name, const sf::Texture& texture, std::vector<Action>&& actions)
: m_name{name}
, m_sprite{texture}
, m_actions{std::forward<std::vector<Action>>(actions)}
{
}

void Screen::update(sf::Time dt)
{

}

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}

void Screen::connect(thor::ActionMap<std::string>::CallbackSystem& callback_system)
{
    //callback_system.connect("click", &WardrobeScreen::check_action);
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
