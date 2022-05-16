#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "enemy.h"      // for keeping enemy
#include "player.h"     // for keeping player
#include "emf.h"

#include "userio.h"     // for getting key press
#include "myListener.h" // for listening collisions
#include "world.h"
#include "camera.h"     // for drawing view

#include <set> // for string objects

class UserIO;

class GameState
{
    std::set<std::shared_ptr<Object>> objects_;
    bool debugMode_{false};

    // Creating world without gravity
    b2World world_{b2Vec2(0.0f, 0.0f)};
    World gameMap_;
    MyListener listener_;

    Player player_;
    Enemy enemy_;

    void debugUpdate(UserIO &userIO);
    bool debugGet() const { return debugMode_; }
    void debugSet(bool debug) { debugMode_ = debug; }

public:
    template <typename T>
    std::shared_ptr<T> shareObject(const T *searchedObj) const
    {
        for (auto obj : objects_)
        {
            if (obj.get() == searchedObj)
            {
                return std::static_pointer_cast<T>(obj);
            }
        }

        return nullptr;
    }

    GameState();
    void addObject(std::shared_ptr<Object> object);
    void step(UserIO &userIO);
};

#endif