#include "world.h"
#include <iostream>

World::World(b2World &world, int size)
{
    for (float i = 0; i < size * Conf::chunkWidth; i += Conf::chunkWidth)
        for (float j = 0; j < size * Conf::chunkWidth; j += Conf::chunkWidth)
        {
            this->spawnChunk(world, b2Vec2(i, j));
        }
}

void World::spawnChunk(b2World &world, const b2Vec2 &position)
{
    chunks[position] = std::make_unique<Chunk>(world, position);
}

void World::removeChunk(const b2Vec2 &position)
{
    b2Vec2 foundPosition = this->closestChunk(position);

    if (foundPosition.IsValid())
        chunks.erase(foundPosition);
};

void World::clear()
{
    chunks.clear();
}

void World::draw(b2World &world, const b2Vec2 &playerPosition)
{
    for (auto &chunk : chunks)
    {
        b2Vec2 distance = chunk.second.get()->getPosition() - playerPosition;
        if (abs(distance.x) < Conf::renderDistance && abs(distance.y) < Conf::renderDistance)
        {
            if (chunk.second.get()->wasCleared())
                chunk.second.get()->restore(world);
        }
        else if (!chunk.second.get()->wasCleared())
            chunk.second.get()->clear();
    }
}

b2Vec2 World::closestChunk(const b2Vec2 &position) const
{
    int normalizedX = round(position.x / Conf::chunkWidth) * Conf::chunkWidth;
    int normalizedY = round(position.y / Conf::chunkWidth) * Conf::chunkWidth;
    b2Vec2 chunkPosition = b2Vec2(normalizedX, normalizedY);

    try
    {
        chunks.at(chunkPosition); // If chunk exists, return it
        return chunkPosition;
    }
    catch (const std::out_of_range &exception)
    {
    }
    return b2Vec2(INFINITY, INFINITY);
}

b2Vec2 World::openChunk(const b2Vec2 &position, Directions &prefDirection) const
{
    Directions unprefDirection = static_cast<Directions>(prefDirection * (-1)); // Reverses preffered direction

    std::vector<std::pair<Directions, b2Vec2>> routes;
    routes.reserve(4);

    b2Vec2 currentChunk = this->closestChunk(position);

    if (currentChunk.IsValid())
    {
        b2Vec2 neighbourChunk = this->closestChunk(b2Vec2(position.x + Conf::chunkWidth, position.y));
        if (neighbourChunk.IsValid() && !chunks.at(currentChunk)->wallNorth) // Checks north route
            routes.push_back({N, neighbourChunk});

        neighbourChunk = this->closestChunk(b2Vec2(position.x, position.y - Conf::chunkWidth));
        if (neighbourChunk.IsValid() && !chunks.at(currentChunk)->wallWest) // Checks west route
            routes.push_back({W, neighbourChunk});

        neighbourChunk = this->closestChunk(b2Vec2(position.x - Conf::chunkWidth, position.y));
        if (neighbourChunk.IsValid() && !chunks.at(neighbourChunk)->wallNorth) // Checks south route
            routes.push_back({S, neighbourChunk});

        neighbourChunk = this->closestChunk(b2Vec2(position.x, position.y + Conf::chunkWidth));
        if (neighbourChunk.IsValid() && !chunks.at(neighbourChunk)->wallWest) // Checks east route
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
