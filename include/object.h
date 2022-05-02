#ifndef OBJECT_H
#define OBJECT_H

#include "shapes.h"      // for shapeIdx_ attribute
#include <box2d/box2d.h> // for creating body

#include <memory>     // for b2Body smart pointer
#include <functional> // for storing fixture calls

class Object
{
public:
    enum ObjectType
    {
        WALL,
        WALL2,
        PLAYER,
        ENEMY,
        ITEM,
        TOTAL
    };

private:
    enum class BodyType
    {
        STATIC,
        DYNAMIC
    };

    // Object generating methods
    static const b2BodyDef &getBodyDef(BodyType bodyType);
    static const b2FixtureDef *getFixtureDef(const b2Vec2 &size);
    static const b2FixtureDef *getFixtureDef(float radius);
    // Returns unique position helping avoiding spawning objects at the same place
    b2Vec2 getNewPosition() const;

    // Object generation properties
    static const std::function<const b2FixtureDef *()> fixtureCalls[ObjectType::TOTAL];
    const static constexpr Shapes::Type shapeIdx[ObjectType::TOTAL]{
        Shapes::WALL, Shapes::RED_WALL, Shapes::PLAYER, Shapes::ENEMY, Shapes::ENEMY};

    static constexpr BodyType bodyTypes[ObjectType::TOTAL]{
        BodyType::STATIC, BodyType::STATIC, BodyType::DYNAMIC, BodyType::DYNAMIC, BodyType::STATIC};

    const Shapes::Type shapeIdx_;

protected:
    std::unique_ptr<b2Body> body_{nullptr};

    virtual ~Object(){
        if (body_)
            destroyBody();
    }
    Object(b2World &world, ObjectType type);
    Object(b2World &world, ObjectType type, const b2Vec2 &position, float angle);
    void setBody(b2World &world, ObjectType type, const b2Vec2 &position, float angle);
    void destroyBody();
    void setSensor(bool sensor);

public:
    Shapes::Type getShapeIdx() const { return shapeIdx_; }
    const b2Vec2 &getPosition() const { return body_->GetPosition(); };
    float getAngle() const { return body_->GetAngle(); };
};

#endif
