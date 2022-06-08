#ifndef PLAYER_H
#define PLAYER_H

#include "userio.h" // for getting mouse input
#include "object.h" // for inheriting form Object
#include "myMath.h" // for rotating vector
#include "item.h"   // for picking up item
#include "world.h"  // for getting items

#include <box2d/box2d.h> // for setting b2Body velocity
#include <vector>        // for storing visible objects
#include <memory>        // for storing items in shared ptr

#include <iostream> // DEBUG

class Player : public Object
{
private:
    // Sets velocity based on the local cooridinates
    void setLocalVelocity(const b2Vec2 &newVelocity);
    void move(float frameDurationMul);
    void lookAround(UserIO &userIO);
    void itemOperations(UserIO &userIO);

    // Stores any items that are near
    std::vector<const Object2D *> visibleObjects_;
    std::shared_ptr<Item> nearbyItem_;
    std::vector<std::shared_ptr<Item>> ownedItems_;
    unsigned int currentItemIdx_{0};

    b2Vec2 currentChunk_{b2Vec2(INFINITY, INFINITY)};

public:
    Player(b2World &world, const b2Vec2 &position, float angle);

    void control(UserIO &userIO, float frameDurationMul);
    void lookAt(const b2Vec2 &target);

    const std::vector<const Object2D *> &getVisibleObjects() const;
    const std::vector<std::shared_ptr<Item>> &getOwnedItems() const { return ownedItems_; }
    const Item *getCurrentItem() const { return ownedItems_[currentItemIdx_].get(); }
    const std::shared_ptr<Item> getNearbyItem() const { return nearbyItem_; }
    void doItemAction() const;

    void objectObserved(const Object2D *object);
    void objectLost(const Object2D *object);

    void itemContact(std::shared_ptr<Item> item);
    void itemLost();

    b2Vec2 &getCurrentChunk() { return currentChunk_; }
    void setCurrentChunk(const b2Vec2 &chunkPosition) { currentChunk_ = chunkPosition; }
};

#endif
