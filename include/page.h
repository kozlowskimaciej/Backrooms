#ifndef PAGE_H
#define PAGE_H

#include "item.h" // for inheritance

class Page : public Item
{
private:
    static unsigned int pickedPages_;

public:
    Page(b2World &world, const b2Vec2 &position, float angle, Mediator &mediator);
    void action(const b2Vec2 &playerPos) override;
    void drop(b2World &world, const Object &player) override;
    void picked() override;
    static unsigned int pickedPages() { return pickedPages_; };
};

#endif
