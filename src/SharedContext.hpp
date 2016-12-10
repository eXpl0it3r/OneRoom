#pragma once

#include <Thor/Resources/ResourceHolder.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

struct SharedContext
{
    sf::RenderWindow window = {{1024, 768}, "One Game", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings{0, 0, 8}};
    thor::ResourceHolder<sf::Texture, std::string> textures;
    thor::ResourceHolder<sf::Font, std::string> fonts;
};
