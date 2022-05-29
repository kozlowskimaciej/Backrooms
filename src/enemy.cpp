#include "enemy.h"

Enemy::Enemy(b2World &world, b2Vec2 position, float angle)
    : Object2D{world, Type::ENEMY, position, angle}
{
    // Start without hunt
    stopHunt();
}

void Enemy::setVelocity(float frameDurationMul)
{
    // Set velocity to waypoints location
    b2Vec2 waypointDirection{getVec(getPosition(), waypoint_)};
    waypointDirection.Normalize();
    waypointDirection *= Conf::linearEnemyVelocity * frameDurationMul;
    body_->SetLinearVelocity(waypointDirection);
}

void Enemy::updateWaypoint(const b2Vec2 &playerPos, const World &gameMap, bool debug)
{
    b2Vec2 ray{getVec(getPosition(), playerPos)};
    Ray::RayCallback rayCallback;
    Ray::sendRay(rayCallback, *body_->GetWorld(), getPosition(), ray);
    if (!rayCallback.hit() && !debug) // DEBUG
    {
        // If player is visible for enemy set waypoint to the player positioin
        waypoint_ = playerPos;

        // Make hunt longer
        huntTimer_.reset();
    }
    else if (distance(getPosition(), waypoint_) < 0.01)
    {
        // If waypoint is reached set waypoint to random location
        const b2Vec2 &newWaypoint{gameMap.openChunk(getPosition(), direction)};
        if (newWaypoint.IsValid())
            waypoint_ = newWaypoint;
    }
}

void Enemy::updateHuntTimer()
{
    if (huntTimer_.elapsed() > Conf::huntDuration)
        stopHunt();
}

void Enemy::control(const b2Vec2 &playerPos, const World &gameMap, float frameDurationMul, bool debug)
{
    if (spawned_)
    {
        updateWaypoint(playerPos, gameMap, debug);
        setVelocity(frameDurationMul);
        updateHuntTimer();
    }
}

void Enemy::startHunt(b2World &world, const Object &player)
{
    // Check if there isn't hunt already
    if (!spawned_)
    {
        spawned_ = true;
        b2Vec2 offset(Conf::chunkWidth, 0.0f); // Needed to not spawn enemy directly on player
        setBody(world, Type::ENEMY, player.getPosition() + offset, 0.0f);

        huntTimer_.reset();
    }
}

void Enemy::stopHunt()
{
    spawned_ = false;
    destroyBody();
}
