#include "player.h"

void Player::control(UserIO &userIO)
{
    // const float VELOCITY = 2.0f;
    static constexpr float LINEAR_VELOCITY = 3.0f;
    static constexpr float ANGULAR_VELOCITY = 0.15f;
    static constexpr float SPRINT_MULTIPLIER = 2.0f;
    b2Vec2 newVelocity(0.0f, 0.0f);

    // Moving objects
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

    int mouseXMovement(userIO.getMouseXMovement());
    body_->SetAngularVelocity(ANGULAR_VELOCITY * mouseXMovement);
}

void Player::setLocalVelocity(const b2Vec2 &newVelocity)
{
    const float angle{body_->GetAngle()};
    body_->SetLinearVelocity(b2Vec2(
        newVelocity.x * sin(angle) + newVelocity.y * cos(angle),
        -newVelocity.x * cos(angle) + newVelocity.y * sin(angle)));
}