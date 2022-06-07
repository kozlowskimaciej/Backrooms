#ifndef OBJECT_H
#define OBJECT_H

#include "myMath.h"
#include "config.h"

#include <box2d/box2d.h> // for creating body

#include <memory> // for b2Body smart pointer
#include <map>    // for collition mask

// Collidable
class Object
{
public:
    enum class Type
    {
        WALL,
        RED_WALL,
        PLAYER,
        ENEMY,
        EMF,
        ITEM_PICK_AREA,
        CAMERA,
        PAGE,
        TOTAL
    };

protected:
    enum Category : uint16
    {
        DEFAULT = 0x0,
        WALL = 0x1,
        PLAYER = 0x2,
        CAMERA = 0x4,
        ENEMY = 0x8,
        ITEM = 0x10,
        ITEM_PICK_AREA = 0x20
    };

private:
    static const std::map<Type, Category> categoryMap_;
    static const std::map<Category, uint16> collisionMask_;

    // Object generating methods
    static const b2BodyDef &getBodyDef(b2BodyType bodyType);
    static std::unique_ptr<b2Shape> getShape(float halfX, float halfY);
    static std::unique_ptr<b2Shape> getShape(float radius);
    static std::unique_ptr<b2Shape> getShape(float FOVangle, float renderDist, int verticesCount);
    static const b2FixtureDef &getFixtureDef(std::unique_ptr<b2Shape> shape);
    // Returns unique position helping avoiding spawning objects at the same place
    b2Vec2 getNewPosition() const;

    friend class Arguments;

protected:
    struct Arguments
    {
        b2BodyType bodyType_;
        b2FixtureDef fixDef_;
        Arguments(b2BodyType bodyType, std::unique_ptr<b2Shape> shape);
    };

    // Arguments for creating objects
    static const Arguments argList[static_cast<int>(Type::TOTAL)];

    std::unique_ptr<b2Body> body_{nullptr};

    virtual ~Object();
    Object(b2World &world, Type type);
    Object(b2World &world, Type type, const b2Vec2 &position, float angle);
    void setBody(b2World &world, Type type, const b2Vec2 &position, float angle);
    void destroyBody();

    // These functions affect only last created fixture
    void addFixture(Type type, Category cat, bool sensor = false);
    void setSensor(bool sensor);
    void setCollisionFilter(Category category) const;

public:
    bool isSensor() const { return body_->GetFixtureList()->IsSensor(); }
    void stop() const { body_->SetLinearVelocity({0.f, 0.f}); }

    const b2Vec2 &getPosition() const { return body_->GetPosition(); };
    float getAngle() const { return body_->GetAngle(); };
    const b2World &getWorld() const { return *body_->GetWorld(); }
};

#endif
