#ifndef CHUNK_H
#define CHUNK_H

#include "object3d.h"
#include "random_generator.h"
#include "myMath.h"
#include "config.h"
#include "mediator.h" // for notifying about item creation
#include "emf.h"
#include "page.h"

#include <limits>
#include <ctime>
#include <random>

class Chunk
{
    friend class World;

private:
    const b2Vec2 position_;
    std::unique_ptr<Object3D>
        wallNorth_{nullptr};
    std::unique_ptr<Object3D> wallWest_{nullptr};
    bool hunt_{false};

    static std::mt19937 mt;
    static RandomGenerator chunkGenerator;
    static std::vector<b2Vec2> spawnedItems_;

    b2Vec2 getWallNorthPosition() const;
    b2Vec2 getWallWestPosition() const;

    template <typename T>
    void spawnItem(unsigned int chance, b2World &world, Mediator &mediator)
    {
        bool willSpawn = chunkGenerator.drawLots(chance, 100u);
        if (willSpawn && std::find(Chunk::spawnedItems_.begin(), Chunk::spawnedItems_.end(), position_) == Chunk::spawnedItems_.end())
        {
            mediator.notify(std::make_shared<T>(world, position_, 0, mediator), Mediator::ITEM_CREATED);
            Chunk::spawnedItems_.push_back(position_);
        }
    };

    void spawnWalls(bool lowerDensity, b2World &world);

public:
    Chunk(b2World &world, const b2Vec2 &position, Mediator &mediator);
    bool isHunt() const;
    b2Vec2 getPosition() const { return position_; };
};

#endif
