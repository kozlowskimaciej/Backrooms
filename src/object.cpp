#include "object.h"

const std::map<Object::Category, uint16> Object::collisionMask_{
    {DEFAULT, 0xFFFF},
    {WALL, PLAYER | ENEMY},
    {PLAYER, WALL | ENEMY | ITEM_PICK_AREA},
    {CAMERA, ENEMY | ITEM},
    {ENEMY, WALL | PLAYER | CAMERA},
    {ITEM, CAMERA},
    {ITEM_PICK_AREA, PLAYER}};

const std::map<Object::Type, Object::Category> Object::categoryMap_{
    {Type::WALL, WALL},
    {Type::RED_WALL, WALL},
    {Type::PLAYER, PLAYER},
    {Type::ENEMY, ENEMY},
    {Type::EMF, ITEM},
    {Type::PAGE, ITEM},
    {Type::CAMERA, CAMERA},
    {Type::ITEM_PICK_AREA, ITEM_PICK_AREA}};

const Object::Arguments Object::argList[static_cast<int>(Type::TOTAL)]{
    {b2_staticBody, getShape(Conf::chunkWidth / 2 + Conf::wallWidth / 2, Conf::wallWidth / 2)},
    {b2_staticBody, getShape(Conf::chunkWidth / 2 + Conf::wallWidth / 2, Conf::wallWidth / 2)},
    {b2_dynamicBody, getShape(.8f)},
    {b2_dynamicBody, getShape(.8f)},
    {b2_staticBody, getShape(0.1f)},
    {b2_staticBody, getShape(10.0f)},
    {b2_staticBody, getShape(Conf::FOVangle, Conf::renderDistance, 5)},
    {b2_dynamicBody, getShape(.1f)}};

Object::Arguments::Arguments(b2BodyType bodyType, std::unique_ptr<b2Shape> shape)
    : bodyType_{bodyType}, fixDef_{getFixtureDef(std::move(shape))} {}

b2Vec2 Object::getNewPosition() const
{
    static int count{0};
    int tmp{count++};
    return b2Vec2(0, tmp * 50.0f);
}

const b2BodyDef &Object::getBodyDef(b2BodyType bodyType)
{
    static b2BodyDef bodyDef;
    bodyDef.type = bodyType;
    bodyDef.position.SetZero();
    return bodyDef;
}

std::unique_ptr<b2Shape> Object::getShape(float halfX, float halfY)
{
    auto shape = std::make_unique<b2PolygonShape>();
    shape->SetAsBox(halfX, halfY);
    return shape;
}

std::unique_ptr<b2Shape> Object::getShape(float radius)
{
    auto shape = std::make_unique<b2CircleShape>();
    shape->m_radius = radius;
    return shape;
}

std::unique_ptr<b2Shape> Object::getShape(float FOVangle, float renderDist, int verticesCount)
{
    auto vertices = std::make_unique<b2Vec2[]>(verticesCount);

    for (int i{1}; i < verticesCount; ++i)
    {
        float angle{FOVangle / 2.0f / (verticesCount - 2) * (2 * i - verticesCount)};
        vertices[i] = renderDist * getVecN(angle);
    }

    auto shape = std::make_unique<b2PolygonShape>();
    shape->Set(vertices.get(), verticesCount);

    return shape;
}

const b2FixtureDef &Object::getFixtureDef(std::unique_ptr<b2Shape> shape)
{
    static b2FixtureDef fixture; // Fixture
    fixture.shape = shape.get();
    shape.release();
    fixture.density = 1.0f;
    fixture.friction = 0.0f;
    return fixture;
}
Object::~Object()
{
    if (body_)
        destroyBody();
}
Object::Object(b2World &world, Type type)
    : Object{world, type, getNewPosition(), 0} {}

Object::Object(b2World &world, Type type, const b2Vec2 &position, float angle)
{
    setBody(world, type, position, angle);
}

void Object::setBody(b2World &world, Type type, const b2Vec2 &position, float angle)
{
    int argIdx{static_cast<int>(type)};
    body_ = std::unique_ptr<b2Body>(world.CreateBody(&getBodyDef(argList[argIdx].bodyType_)));
    body_->SetTransform(position, angle);
    body_->CreateFixture(&argList[argIdx].fixDef_);

    // Setting proper collision filter
    setCollisionFilter(categoryMap_.at(type));

    // Setting Object as user data
    body_->GetUserData().pointer = (uintptr_t)this;
}

void Object::destroyBody()
{
    body_->GetWorld()->DestroyBody(body_.get());
    body_.release(); // This body is no longer valid
}

void Object::addFixture(Type type, Category cat, bool sensor)
{
    body_->CreateFixture(&argList[static_cast<int>(type)].fixDef_);

    setCollisionFilter(cat);
    setSensor(sensor);
}

void Object::setSensor(bool sensor)
{
    body_->GetFixtureList()->SetSensor(sensor);
}

void Object::setCollisionFilter(Category category) const
{
    b2Filter filter;
    filter.categoryBits = static_cast<uint16>(category);
    filter.maskBits = collisionMask_.at(category);
    body_->GetFixtureList()->SetFilterData(filter);
}
