#include "world.h"
#include <iostream>

b2Vec2 World::normalizeChunkPosition(b2Vec2 position) const
{
    position.x = round(position.x / Conf::chunkWidth) * Conf::chunkWidth;
    position.y = round(position.y / Conf::chunkWidth) * Conf::chunkWidth;

    return position;
}

/**
 * @brief
 * Spawns chunk in given position
 *
 * @param world
 * World in which the chunk will be spawned
 * @param position
 * Position of to be spawned chunk
 */
void World::spawnChunk(b2World &world, const b2Vec2 &position, Mediator &mediator)
{
    chunks_[position] = std::make_unique<Chunk>(world, position, mediator);
}

/**
 * @brief
 * Removes chunk from given position
 *
 * @param position
 * Position of to be removed chunk
 */
void World::removeChunk(const b2Vec2 &position)
{
    b2Vec2 foundPosition = closestChunk(position);

    if (foundPosition.IsValid())
        chunks_.erase(foundPosition);
}

/**
 * @brief
 * Removes all chunks_
 */
void World::clear()
{
    chunks_.clear();
}

/**
 * @brief
 * Renders chunks_ in render distance field and removes unrendered chunks_
 * @param world
 * World with chunks_ that will be altered
 * @param playerPosition
 * Position of player, from whom will be calculated distance to surrounding chunks_
 */
void World::draw(b2World &world, const b2Vec2 &playerPosition, Mediator &mediator)
{
    b2Vec2 normalizedPosition = normalizeChunkPosition(playerPosition);

    float removeFactor = Conf::renderChunkDistance * 2;
    float renderFactor = Conf::renderChunkDistance / 2;

    for (float i = normalizedPosition.x - Conf::chunkWidth * removeFactor; i <= normalizedPosition.x + Conf::chunkWidth * removeFactor; i += Conf::chunkWidth)
        for (float j = normalizedPosition.y - Conf::chunkWidth * removeFactor; j <= normalizedPosition.y + Conf::chunkWidth * removeFactor; j += Conf::chunkWidth)
        {
            try
            {
                chunks_.at(b2Vec2(i, j));

                if (i < normalizedPosition.x - Conf::chunkWidth * Conf::renderChunkDistance || i > normalizedPosition.x + Conf::chunkWidth * Conf::renderChunkDistance)
                    chunks_.erase(b2Vec2(i, j));
                if (j < normalizedPosition.y - Conf::chunkWidth * Conf::renderChunkDistance || j > normalizedPosition.y + Conf::chunkWidth * Conf::renderChunkDistance)
                    chunks_.erase(b2Vec2(i, j));
            }
            catch (const std::out_of_range &exception)
            {
                if (i >= normalizedPosition.x - Conf::chunkWidth * renderFactor || i <= normalizedPosition.x + Conf::chunkWidth * renderFactor)
                    if (j >= normalizedPosition.y - Conf::chunkWidth * renderFactor || j <= normalizedPosition.y + Conf::chunkWidth * renderFactor)
                        spawnChunk(world, b2Vec2(i, j), mediator);
            }
        }
}

bool World::isHunt(const b2Vec2 &position) const
{
    b2Vec2 chunkPosition = normalizeChunkPosition(position);

    try
    {
        return chunks_.at(chunkPosition).get()->isHunt();
    }
    catch (const std::out_of_range &exception)
    {
    }
    return false;
}

/**
 * @brief
 * Returns closest chunk's position to a given position
 *
 * @param position
 * @return b2Vec2
 * Position of closest chunk (INF, INF if chunk does not exist)
 */
b2Vec2 World::closestChunk(const b2Vec2 &position) const
{
    b2Vec2 chunkPosition = normalizeChunkPosition(position);
    try
    {
        chunks_.at(chunkPosition); // If chunk exists, return it
        return chunkPosition;
    }
    catch (const std::out_of_range &exception)
    {
    }
    return b2Vec2(INFINITY, INFINITY);
}

/**
 * @brief
 * Returns random neighbouring chunk which we can access from given position
 *
 * @param position
 * @param prefDirection
 * Disables randomizing if possible (chunk is accessible)
 *
 * @return b2Vec2
 * Returns chunk's position (INF, INF if chunk does not exist)
 */

b2Vec2 World::openChunk(const b2Vec2 &position, Directions &prefDirection) const
{
    Directions unprefDirection = static_cast<Directions>(prefDirection * (-1)); // Reverses preffered direction

    std::vector<std::pair<Directions, b2Vec2>> routes;
    routes.reserve(4);

    b2Vec2 currentChunk = this->closestChunk(position);

    if (currentChunk.IsValid())
    {
        b2Vec2 neighbourChunk = this->closestChunk(b2Vec2(position.x + Conf::chunkWidth, position.y));
        if (neighbourChunk.IsValid() && !chunks_.at(currentChunk)->wallNorth_) // Checks north route
            routes.push_back({N, neighbourChunk});

        neighbourChunk = this->closestChunk(b2Vec2(position.x, position.y - Conf::chunkWidth));
        if (neighbourChunk.IsValid() && !chunks_.at(currentChunk)->wallWest_) // Checks west route
            routes.push_back({W, neighbourChunk});

        neighbourChunk = this->closestChunk(b2Vec2(position.x - Conf::chunkWidth, position.y));
        if (neighbourChunk.IsValid() && !chunks_.at(neighbourChunk)->wallNorth_) // Checks south route
            routes.push_back({S, neighbourChunk});

        neighbourChunk = this->closestChunk(b2Vec2(position.x, position.y + Conf::chunkWidth));
        if (neighbourChunk.IsValid() && !chunks_.at(neighbourChunk)->wallWest_) // Checks east route
            routes.push_back({E, neighbourChunk});

        if (!routes.size()) // If no choice is available, return not valid vector
            return b2Vec2(INFINITY, INFINITY);

        if (routes.size() > 1) // If there is more than one choice available, remove unpreffered direction from choices pool
        {
            std::remove_if(
                routes.begin(), routes.end(), [unprefDirection](auto &route)
                { return unprefDirection == route.first; });
        }

        int choice = int(Chunk::mt() % 2); // Choice for choosing preffered route

        if (!choice) // If preffered direction is available as choice (and choice equals 0), return it
        {
            auto it = std::find_if(routes.begin(), routes.end(), [prefDirection](auto &route)
                                   { return prefDirection == route.first; });

            if (it != routes.end())
            {
                prefDirection = it->first;
                return it->second;
            }
        }

        choice = int(Chunk::mt() % routes.size()); // Choice for choosing from available routes if preffered route was not taken

        prefDirection = routes[choice].first; // Set new preffered route and return chunk's coordinates
        return routes[choice].second;
    }
    return b2Vec2(INFINITY, INFINITY);
}
