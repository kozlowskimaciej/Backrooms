#include "player.h"

void Player::setLocalVelocity(const b2Vec2 &newVelocity)
{
    const float angle{body_->GetAngle()};
    body_->SetLinearVelocity(rotateVec(newVelocity, angle));
}

Player::Player(b2World &world, const b2Vec2 &position, float angle)
    : Object{world, Type::PLAYER, position, angle}
{
    // Collision filter for PLAYER is set in Object's constructor

    // New fixture is added at the front
    addFixture(Type::CAMERA, Category::CAMERA, true);

    // Changing camera mass to zero
    body_->GetFixtureList()->SetDensity(0.0f);
    body_->ResetMassData();
}

void Player::move(float frameDurationMul)
{
    static constexpr float SPRINT_MULTIPLIER = 2.0f;
    float linearVelocity = Conf::linearPlayerVelocity * frameDurationMul;
    b2Vec2 newVelocity(0.0f, 0.0f);

    // Moving
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        newVelocity += b2Vec2(0.0f, linearVelocity);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        newVelocity += b2Vec2(0.0f, -linearVelocity);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        newVelocity += b2Vec2(linearVelocity, 0.0f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        newVelocity += b2Vec2(-linearVelocity, 0.0f);
    }

    newVelocity.Normalize();
    newVelocity *= linearVelocity;

    // Sprint only when going forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        newVelocity *= SPRINT_MULTIPLIER;
    }

    setLocalVelocity(newVelocity);
}

void Player::lookAround(UserIO &userIO)
{
    static constexpr float ANGULAR_VELOCITY = 0.15f;

    // looking around
    int mouseXMovement(userIO.getMouseXMovement());
    body_->SetAngularVelocity(ANGULAR_VELOCITY * mouseXMovement);
}

void Player::lookAt(const b2Vec2 &target)
{
    float angle = vecAngle(getVec(getAngle()), getVec(getPosition(), target));
    float cross = b2Cross(getVec(getAngle()), getVec(getPosition(), target));
    angle *= abs(cross) / cross;
    body_->SetAngularVelocity(angle);
}

void Player::itemOperations(UserIO &userIO)
{
    // Pickig up items
    if (userIO.handleKeyPress(sf::Keyboard::E) && nearbyItem_)
    {
        ownedItems_.push_back(nearbyItem_);
        nearbyItem_->picked();
        nearbyItem_->destroyBody();
        // Deleting b2Body calls EndContact
        // but this is likely done by another "thread"
        // So Item is automatically removed from visibleObjects_ and nearbyItem_
    }

    // Dropping item
    if (userIO.handleKeyPress(sf::Keyboard::G) && ownedItems_.size())
    {
        ownedItems_[currentItemIdx_]->drop(*body_->GetWorld(), *this);
        ownedItems_.erase(std::next(ownedItems_.begin(), currentItemIdx_));
        if (currentItemIdx_ == ownedItems_.size())
        {
            currentItemIdx_ = 0;
        }
    }

    // Changing item from inventory
    if (userIO.handleKeyPress(sf::Keyboard::Q) && ++currentItemIdx_ >= ownedItems_.size())
        currentItemIdx_ = 0;
}

void Player::control(UserIO &userIO, float frameDurationMul)
{
    move(frameDurationMul);
    lookAround(userIO);
    itemOperations(userIO);

    // Sorting visible objects by distance from the player
    const b2Vec2 playerPosition{getPosition()};
    std::sort(visibleObjects_.begin(), visibleObjects_.end(), [&playerPosition](const Object2D *objA, const Object2D *objB)
              { return distance(playerPosition, objA->getPosition()) > distance(playerPosition, objB->getPosition()); });
}

const std::vector<const Object2D *> &Player::getVisibleObjects() const
{
    return visibleObjects_;
}

void Player::doItemAction() const
{
    for (auto &&item : ownedItems_)
        item->action(getPosition());
}

void Player::objectObserved(const Object2D *object)
{
    visibleObjects_.push_back(object);
}

void Player::objectLost(const Object2D *object)
{
    auto foundObj = std::find(visibleObjects_.begin(), visibleObjects_.end(), object);
    if (foundObj != visibleObjects_.end())
        visibleObjects_.erase(foundObj);
}

void Player::itemContact(std::shared_ptr<Item> item)
{
    assert(item);

    nearbyItem_ = std::move(item);
}

void Player::itemLost()
{
    nearbyItem_ = nullptr;
}
