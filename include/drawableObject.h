#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "object.h"
#include "image.h" // for shapeIdx_ attribute

#include <map>

class DrawableObject : public Object
{
private:
    const Image::Type shapeIdx_;
    static std::map<Object::Type, Image::Type> shapesMap_;

protected:
    DrawableObject(b2World &world, Type type, const b2Vec2 &position, float angle);

public:
    Image::Type getShapeIdx() const { return shapeIdx_; }
};

#endif
