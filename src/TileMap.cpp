#include "TileMap.hpp"

#include "json.hpp"
#include <Thor/Resources.hpp>
#include <fstream>

TileMap::TileMap(SharedContext& context, const std::string& filename)
: m_context(context)
{
    load_map(filename);
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(auto& tile : m_tiles)
        target.draw(tile, states);
}

sf::Vector2f TileMap::spawn()
{
    return m_spawn;
}

sf::FloatRect TileMap::check_collision(sf::FloatRect bounds)
{
    /*std::vector<std::size_t> collision_indices;
    collision_indices.emplace_back(tile_index({bounds.left, bounds.top}));
    collision_indices.emplace_back(tile_index({bounds.left + bounds.width, bounds.top}));
    collision_indices.emplace_back(tile_index({bounds.left - bounds.width, bounds.top}));
    collision_indices.emplace_back(tile_index({bounds.left, bounds.top + bounds.height}));
    collision_indices.emplace_back(tile_index({bounds.left + bounds.width, bounds.top + bounds.height}));
    collision_indices.emplace_back(tile_index({bounds.left, bounds.top}));
*/

    sf::Vector2f pos{bounds.left, bounds.top};
    auto pos_lt = tile_position({bounds.left, bounds.top});
    auto pos_rt = tile_position({bounds.left + bounds.width, bounds.top});
    auto pos_lb = tile_position({bounds.left, bounds.top + bounds.height});
    auto pos_rb = tile_position({bounds.left + bounds.width, bounds.top + bounds.height});

    /*if(pos.x < pos_lt.x)
        if(pos.x < pos_lt.x)*/

    bool collision = false;
    sf::Vector2f reset_pos{bounds.left, bounds.top};

    if(m_logic[tile_index(pos_lt)] > LogicTiles::COLLIDABLE && bounds.left > (pos_lt.x + 1) * m_tile_size) // || bounds.left > (pos_lb.x + 1) * m_tile_size)
    {
        reset_pos.x = (pos_lt.x + 1) * m_tile_size;
        collision = true;
    }

    if(m_logic[tile_index(pos_lt)] > LogicTiles::COLLIDABLE && bounds.top < (pos_lt.y + 1) * m_tile_size) // || bounds.top < (pos_rt.y + 1) * m_tile_size)
    {
        reset_pos.y = (pos_lt.y + 1) * m_tile_size;
        collision = true;
    }

    if(m_logic[tile_index(pos_rt)] > LogicTiles::COLLIDABLE && bounds.left + bounds.width < (pos_rt.x + 1) * m_tile_size) // || bounds.left + bounds.width < (pos_rb.x + 1) * m_tile_size)
    {
        reset_pos.x = (pos_rt.x - 1) * m_tile_size;
        collision = true;
    }

    if(m_logic[tile_index(pos_lb)] > LogicTiles::COLLIDABLE && bounds.top + bounds.height < (pos_lb.y + 1) * m_tile_size) // || bounds.top + bounds.height < (pos_rb.y + 1) * m_tile_size)
    {
        reset_pos.y = (pos_lt.y - 1) * m_tile_size;
        collision = true;
    }

    if(collision)
    {
        m_last_bounds = {reset_pos.x, reset_pos.y, bounds.width, bounds.height};
    }
    else
    {
        m_last_bounds = bounds;
    }

    /*if(!(m_logic[pos_lt] > LogicTiles::COLLIDABLE
        || m_logic[pos_rt] > LogicTiles::COLLIDABLE
        || m_logic[pos_lb] > LogicTiles::COLLIDABLE
        || m_logic[pos_rb] > LogicTiles::COLLIDABLE))
    {
        std::cout << pos_lt << ", " << pos_rt << ", " << pos_lb << ", " << pos_rb << "\n";
        m_last_bounds = bounds;
    }*/

    return m_last_bounds;
}

sf::Vector2u TileMap::tile_position(sf::Vector2f position)
{
    return static_cast<sf::Vector2u>(sf::Vector2f{position.x, position.y} / static_cast<float>(m_tile_size));
}

std::size_t TileMap::tile_index(sf::Vector2u position)
{
    return (position.y * m_dimension.x) + position.x;
}

void TileMap::load_map(const std::string& filename)
{
    nlohmann::json tilemap;

    std::fstream file(filename, std::ios::in | std::ios::binary);

    if(!file.is_open())
        throw(std::runtime_error{"Failed to open tilemap file \"" + filename + "\"."});

    try
    {
        file >> tilemap;
    }
    catch(std::exception& e)
    {
        throw(std::runtime_error{"The tilemap file \"" + filename + "\" contains invalid JSON data."});
    }

    // Load texture
    std::string texture = tilemap["texture"];
    m_context.textures.acquire("level", thor::Resources::fromFile<sf::Texture>(texture));

    // Load base tiles
    m_tile_size = tilemap["tile_size"];
    std::vector<Tile> base_tiles;

    for(auto& rect : tilemap["tiles"])
    {
        sf::IntRect texture_rect(rect[0], rect[1], static_cast<int>(m_tile_size), static_cast<int>(m_tile_size));
        base_tiles.emplace_back(m_context.textures["level"], texture_rect);
    }

    sf::IntRect texture_rect(tilemap["empty_tile"][0], tilemap["empty_tile"][1],static_cast<int>( m_tile_size), static_cast<int>(m_tile_size));
    m_empty_tile = std::make_unique<Tile>(m_context.textures["level"], texture_rect);

    // Load board
    std::size_t x = 0;
    std::size_t y = 0;

    m_dimension = sf::Vector2u(tilemap["board"].begin()->size(), tilemap["board"].size());

    for(auto& line : tilemap["board"])
    {
        for(std::size_t tile : line)
        {
            // First tile
            if(x == 0 && y == 0)
                m_tiles.resize(m_dimension.y * m_dimension.x, *m_empty_tile);

            std::size_t index = y * m_dimension.x + x;

            // Copy base tile
            m_tiles[index] = base_tiles[tile];
            m_tiles[index].setPosition({static_cast<float>(x * m_tile_size), static_cast<float>(y * m_tile_size)});

            ++x;
        };
        ++y;
        x = 0;
    }

    // Load logic part
    x = 0;
    y = 0;

    for(auto& line : tilemap["logic"])
    {
        for(std::size_t tile : line)
        {
            LogicTiles logic_tile = static_cast<LogicTiles>(tile);

            // First tile
            if(x == 0 && y == 0)
                m_logic.resize(m_dimension.y * m_dimension.x);

            m_logic[y * m_dimension.x + x] = logic_tile;

            if(logic_tile == LogicTiles::SPAWN)
            {
                m_spawn = {static_cast<float>(x * m_tile_size), static_cast<float>(y * m_tile_size)};
                m_last_bounds = {m_spawn.x, m_spawn.y, static_cast<float>(m_tile_size), static_cast<float>(m_tile_size)};
            }

            ++x;
        }
        ++y;
        x = 0;
    }
}
