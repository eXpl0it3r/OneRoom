#pragma once

#include "Action.hpp"
#include <Thor/Input.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Screen : public sf::Drawable, public sf::Transformable
{
public:
    Screen(const std::string& name, const sf::Texture& texture, std::vector<Action>&& actions);

    void update(sf::Time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void connect(thor::ActionMap<std::string>::CallbackSystem&);

    void on_click(sf::Vector2i position);
    void on_hover(sf::Vector2i position);

    std::string name();
    std::string action();
    std::string hovering();

protected:
    std::string m_name;
    sf::Sprite m_sprite;
    std::vector<Action> m_actions;
    std::string m_action;
    std::string m_hovering;
};
