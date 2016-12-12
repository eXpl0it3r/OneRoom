#pragma once

#include "Screen.hpp"
#include <Thor/Input/ActionMap.hpp>
#include <Thor/Resources/ResourceHolder.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

struct SharedContext
{
    sf::RenderWindow window = {{396, 400}, "One Room", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings{0, 0, 8}};
    thor::ResourceHolder<sf::Texture, std::string> textures;
    thor::ResourceHolder<sf::Font, std::string> fonts;
    thor::ActionMap<std::string> actionmap;
    std::shared_ptr<Screen> current_room = nullptr;
    std::vector<int> lock_combination = {2, 7, 6};
};
