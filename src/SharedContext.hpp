#pragma once

#include "Screen.hpp"
#include <Thor/Input.hpp>
#include <Thor/Resources.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

struct SharedContext
{
    sf::RenderWindow window = {{396, 400}, "One Room", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings{0, 0, 8}};
    thor::ResourceHolder<sf::Texture, std::string> textures;
    thor::ResourceHolder<sf::Font, std::string> fonts;
    thor::ResourceHolder<sf::SoundBuffer, std::string> sounds;
    sf::Music track_one;
    thor::ActionMap<std::string> actionmap;
    std::shared_ptr<Screen> current_room = nullptr;
    const std::vector<int> lock_combination = {1, 0, 3};
    const std::vector<int> pin_combination = {3, 2, 7, 6};
    const std::string password = "NSA4u";
    bool key = false;
    bool combination = false;
    bool login = false;
    bool briefcase = false;
    bool win = false;
};
