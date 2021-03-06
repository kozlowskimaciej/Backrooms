#ifndef CAMERA_H
#define CAMERA_H

#include "userio.h"   // called to draw Shape on screen
#include "object2d.h" // for drawing Object2D
#include "myMath.h"   // for angle calculations
#include "ray.h"      // for casting rays
#include "config.h"   // for constants
#include "player.h"   // for getting visible objects
#include "world.h"     // for getting debug information

#include <vector> // for storing Object2Ds
#include <memory>

class Camera
{
private:
    using scale_t = std::pair<float, float>;

    static constexpr int raysNumber_{400};

    static float distDimFactor(float distance, float maxDistance = Conf::renderDistance);
    static float angleDimFactor(const Ray::RayCallback &callback);
    static scale_t get3DScale(float adjacentDistance);
    static scale_t get2DScale(float adjacentDistance);

    // For 3D objects
    static void draw3DRay(UserIO &userIO, bool debug, float angle, const Ray::RayCallback &callback, float rayNumber);
    // For 2D objectsSetSensor
    static void draw2DRay(UserIO &userIO, bool debug, float angle, const Ray::RayCallback &callback, float distance);

    static bool ifInFieldOfView(const Object &camera, const Object &object);
    static void drawObjects3D(UserIO &userIO, bool debug, const Player &player);
    static void drawObjects2D(UserIO &userIO, bool debug, const Player &player);
    static void drawItems(UserIO &userIO, const Player &player);
    static void postFx(UserIO &userIO);
    static void background(UserIO &userIO);

public:
    // Casts multiple rays to show them as image on the screen
    static void drawViewOnScreen(UserIO &userIO, bool debug, const Player &player);
};

#endif
