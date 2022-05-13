#include "camera.h"
#include "object3d.h"
#include "player.h"
#include "userio.h"
#include "textures.h"
#include "chunk.h"
#include "enemy.h"
#include "timer.h"
#include "item.h"
#include "emf.h"
#include "world.h"
#include "myListener.h" // for setting contact listener

#include <vector>   // TEST
#include <iostream> // Printing information about texture loading fail

#include <map>
int main()
{
    // Loading textures
    if (!Textures::init())
    {
        std::cerr << "Loading textures failed\n";
        return -1;
    }
    Shapes::init();

    // User input via mouse and output via screen
    UserIO userIO("Backrooms");

    // Creating world without gravity
    b2World world{b2Vec2(0.0f, 0.0f)};

    // Setting contact listener
    MyListener listener;
    world.SetContactListener(&listener);

    // Creating chunks
    // Objects
    Player player(world, b2Vec2(-15.0f, -15.0f), 0);
    Enemy enemy(world, b2Vec2(0.0f, 0.0f), 0.0f);
    // Emf emf(world, b2Vec2(-5.0f, -15.0f), 0.0f);
    // Item item2(world, Shapes::RED_WALL, b2Vec2(5.0f, -15.0f), 0.0f);

    World gameMap(world, 1, player.getPosition());

    // gameMap.createItem<Emf>(world, Object::Type::EMF, b2Vec2(-5.0f, -15.0f), 0.0f);

    // Simulation parameters
    float timeStep = 1.0f / 60.0f; // Step of time between events
    int32 velocityIterations = 4;  // Velocity calculations during one step
    int32 positionIterations = 3;  // Position calculations during one step
    // Chunk *b = new Chunk(world, b2Vec2(2, 3));
    // delete b;gameMap

    // Main loop
    while (userIO.isOpen())
    {
        // Handling events
        userIO.handleEvents();

        // Physics step
        world.Step(timeStep, velocityIterations, positionIterations);
        player.control(userIO, gameMap);
        player.doItemAction(world);
        enemy.control(player, gameMap);

        // Chunks update
        gameMap.draw(world, player.getPosition());

        // b2Vec2 position = player.getPosition();
        // b2Vec2 playerChunk = gameMap.closestChunk(position);
        // b2Vec2 closestChunk = gameMap.openChunk(position);

        // std::cerr << "\n::PLAYER'S CHUNK::\n";
        // std::cerr << playerChunk.x << " " << playerChunk.y << std::endl;
        // if (closestChunk.IsValid())
        // {

        //     std::cerr << "::CLOSEST OPEN CHUNK::\n";
        //     std::cerr << closestChunk.x << " " << closestChunk.y << std::endl; // DEBUG
        // }

        // Drawing on screen
        Camera::drawViewOnScreen(userIO, gameMap.debugGet(), player);
    }

    return 0;
}
