#include "object.h"

b2Vec2 Object::getNewPosition() const
{
    static int count{0};
    int tmp{count++};
    return b2Vec2(0, tmp * 50.0f);
}

const b2BodyDef &Object::getBodyDef(BodyType bodyType)
{
    static b2BodyDef bodyDef;
    if (bodyType == BodyType::DYNAMIC)
        bodyDef.type = b2_dynamicBody;
    else if (bodyType == BodyType::STATIC)
        bodyDef.type = b2_staticBody;
    else
        throw "Invalid body type\n";
    return bodyDef;
}

const b2FixtureDef *Object::getFixtureDef(const b2Vec2 &size)
{
    static b2PolygonShape rectShape; // Shape
    rectShape.SetAsBox(size.x, size.y);

    static b2FixtureDef fixture; // Fixture
    fixture.shape = &rectShape;
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    return &fixture;
}

const b2FixtureDef *Object::getFixtureDef(float radius)
{
    static b2CircleShape circleShape; // Shape
    circleShape.m_radius = radius;

    static b2FixtureDef fixture; // Fixture
    fixture.shape = &circleShape;
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    return &fixture;
}

Object::Object(ObjectType objectType)
    : shapeIdx_{shapeIdx[objectType]} {}

void Object::setBody(MyWorld &world, ObjectType type)
{
    body_ = createBody(getBodyDef(bodyTypes[type]), world);
    body_->SetTransform(getNewPosition(), 0);
    body_->CreateFixture(fixtureCalls[type]());
}

void Object::setBody(MyWorld &world, ObjectType type, b2Vec2 position, float angle)
{
    body_ = createBody(getBodyDef(bodyTypes[type]), world);
    body_->SetTransform(position, angle);
    body_->CreateFixture(fixtureCalls[type]());
}

const std::function<const b2FixtureDef *()> Object::fixtureCalls[ObjectType::TOTAL]{
    []()
    { return getFixtureDef(b2Vec2(10.0f, 1.0f)); },
    []()
    { return getFixtureDef(b2Vec2(10.0f, 1.0f)); },
    []()
    { return getFixtureDef(0.5f); },
    []()
    { return getFixtureDef(0.5f); }};
