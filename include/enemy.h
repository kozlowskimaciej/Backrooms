#ifndef ENEMY_H
#define ENEMY_H

#include "object2d.h" // for inheriting from Object2D
#include "world.h"    // for getting openChunk waypoints
#include "ray.h"      // for ray sending to check player visibility
#include "timer.h"    // for hunt timer

class Enemy : public Object2D
{
    // Destination where enemy is currently going
    b2Vec2 waypoint_;

    // Time since the player has been last seen
    Timer huntTimer_;
    bool spawned_{true};

    // Sets velocity for current waypoint
    void setVelocity(float frameDurationMul);

    // Updated waypoint to player's position
    // only when player is visible
    void updateWaypoint(const b2Vec2 &playerPos, const World &gameMap, bool debug);
    void updateHuntTimer();

    World::Directions direction = World::Directions::N;

public:
    Enemy(b2World &world, b2Vec2 position, float angle);

    void control(const b2Vec2 &playerPos, const World &gameMap, float frameDurationMul, bool debug);
    void startHunt(b2World &world, const Object &player);
    void stopHunt();

    bool spawned() const { return spawned_; }
};

#endif
