#ifndef EMF_H
#define EMF_H

#include "item.h"   // for inheritance
#include "enemy.h"  // for finding closest enemy
#include "myMath.h" // for calculating distance to enemy

#include <array>    // for storing emf shapes

class Emf : public Item
{
private:
    static constexpr float detectionRadius{50.0f};
    static constexpr unsigned int statesNumber{4};
    static constexpr std::array<Image::Type, statesNumber> emfShapes_{
        Image::EMF,
        Image::EMF1,
        Image::EMF2,
        Image::EMF3};
    // bool on_{false};
    const Enemy &enemy_;

public:
    Emf(b2World &world, const b2Vec2 &position, float angle, const Enemy &enemy);
    void drop(b2World &world, const Object &player) override;

    void action(const b2Vec2 &playerPos) override;
};

#endif
