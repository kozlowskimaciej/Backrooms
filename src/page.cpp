#include "page.h"

unsigned int Page::pickedPages_{0};

Page::Page(b2World &world, const b2Vec2 &position, float angle, Mediator &mediator)
    : Item{world, Type::PAGE, Image::EMPTY, position, angle, mediator} {}

void Page::action(const b2Vec2 &playerPos)
{
    (void)playerPos;
}

void Page::drop(b2World &world, const Object &player)
{
    (void)world;
    (void)player;
}

void Page::picked()
{
    pickedPages_++;
}