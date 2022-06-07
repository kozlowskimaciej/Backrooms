#include "drawableObject.h"

DrawableObject::DrawableObject(b2World &world, Type type, const b2Vec2 &position, float angle)
    : Object{world, type, position, angle}, shapeIdx_{shapesMap_.at(type)} {}

std::map<Object::Type, Image::Type> DrawableObject::shapesMap_{
    {Type::WALL, Image::WALL},
    {Type::RED_WALL, Image::RED_WALL},
    {Type::ENEMY, Image::ENEMY},
    {Type::EMF, Image::EMF_DROPPED},
    {Type::PAGE, Image::PAGE}};
