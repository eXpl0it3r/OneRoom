#pragma once

#include "SharedContext.hpp"
#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class TileMap : public sf::Drawable
{
public:
    enum class LogicTiles {
        SPAWN = 1,
        COLLIDABLE = 10
    };

public:
    TileMap(SharedContext& context, const std::string& filename);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    sf::Vector2f spawn();
    sf::FloatRect check_collision(sf::FloatRect bounds);

private:
    sf::Vector2u tile_position(sf::Vector2f position);
    std::size_t tile_index(sf::Vector2u position);

    void load_map(const std::string& filename);

private:
    SharedContext& m_context;

    std::size_t m_tile_size;
    sf::Vector2u m_dimension;
    std::vector<Tile> m_tiles;
    std::vector<LogicTiles> m_logic;
    std::unique_ptr<Tile> m_empty_tile;

    sf::Vector2f m_spawn;
    sf::FloatRect m_last_bounds;
};
