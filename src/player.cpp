#include "player.h"

void Player::setLocalVelocity(const b2Vec2 &newVelocity)
{
    const float angle{body_->GetAngle()};
    body_->SetLinearVelocity(rotateVec(newVelocity, angle));
}

Player::Player(b2World &world, const b2Vec2 &position, float angle)
    : Object{world, Type::PLAYER, position, angle}
{
    // New fixture is added at the front
    body_->CreateFixture(&argList[static_cast<int>(Type::CAMERA)].fixDef_);

    // Changing camera mass to zero
    body_->GetFixtureList()[0].SetDensity(0.0f);
    body_->ResetMassData();

    setCollisionFilter(Category::PLAYER, 1);
    setCollisionFilter(Category::CAMERA, 0);
    setSensor(true, 0);
}

#include <iostream> // DEBUG

void Player::objectObserved(const Object2D *object)
{
    std::cerr << "object Observed\n";
    visibleObjects_.push_back(object);
}

void Player::objectLost(const Object2D *object)
{
    std::cerr << "object Lost\n";
    auto foundObj = std::find(visibleObjects_.begin(), visibleObjects_.end(), object);
    visibleObjects_.erase(foundObj);
}

void Player::control(UserIO &userIO)
{
    static constexpr float LINEAR_VELOCITY = 2.0f;
    static constexpr float ANGULAR_VELOCITY = 0.15f;
    static constexpr float SPRINT_MULTIPLIER = 2.0f;
    b2Vec2 newVelocity(0.0f, 0.0f);

    // Moving
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        newVelocity += b2Vec2(0.0f, LINEAR_VELOCITY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        newVelocity += b2Vec2(0.0f, -LINEAR_VELOCITY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        newVelocity += b2Vec2(LINEAR_VELOCITY, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        newVelocity += b2Vec2(-LINEAR_VELOCITY, 0.0f);
    }

    newVelocity.Normalize();
    newVelocity *= LINEAR_VELOCITY;

    // Sprint
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
    {
        newVelocity *= SPRINT_MULTIPLIER;
    }

    setLocalVelocity(newVelocity);

    // looking around
    int mouseXMovement(userIO.getMouseXMovement());
    body_->SetAngularVelocity(ANGULAR_VELOCITY * mouseXMovement);

    // Pickig up items
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        // for (auto item : nearbyItems_)
        // {
        //     // delete item
        // }
        nearbyItems_.clear();
    }
}

void Player::itemContact(const Item *item)
{
    nearbyItems_.insert(item);
}

void Player::itemLost(const Item *item)
{
    nearbyItems_.erase(item);
}
