#include "camera.h"
#include <iostream> // DEBUG

inline float Camera::getDimFactor(const MyCallback &callback)
{
    // float dimFactor = 1.0f / callback.getFraction() * vecCosine(callback.normal_, callback.ray_) / 20;
    float dimFactor = (vecCosine(callback.normal_, callback.ray_) * 0.75f + (1.0f / 2.2f)) / 20 * (-callback.getFraction() * rayLength_ / 100 + 1.3f) * 10;
    // std::cerr << dimFactor << '\n'; // DEBUG
    return dimFactor;
    // return 0.4f / callback.getFraction() * vecCosine(callback.normal_, callback.ray_);
}

inline Camera::scale_t Camera::get3DScale(float adjacentDistance)
{
    return {
        (adjacentDistance + 756.52173f) * 0.7827916f / raysNumber_,
        100.0f / adjacentDistance};
}

inline Camera::scale_t Camera::get2DScale(float adjacentDistance)
{
    return {
        28.0f / adjacentDistance / FOVMaxAngle_,
        56.0f / adjacentDistance};
}

#include "timer.h" // DEBUG

void Camera::drawRay(UserIO &userIO, float angle, const MyCallback &callback)
{
    // static Timer t; // DEBUG
    // t.start();      // DEBUG

    // y component of distance measured in local coordinate system
    float adjacentDistance{cos(angle) * callback.getFraction() * rayLength_};
    scale_t scale{get3DScale(adjacentDistance)};

    float dimFactor = getDimFactor(callback);

    static const float maxCordX{tan(FOVMaxAngle_)};
    userIO.drawOnScreen(callback.getShapeIdx(), tan(angle) / maxCordX, 0.0f, scale.first, scale.second, dimFactor);

    // t.stop(); // DEBUG
}

void Camera::drawRay(UserIO &userIO, float angle, const MyCallback &callback, float distance)
{
    // y component of distance measured in local coordinate system
    float adjacentDistance{cos(angle) * callback.getFraction() * distance};
    scale_t scale{get2DScale(adjacentDistance)};

    static const float maxCordX{tan(FOVMaxAngle_)};
    userIO.drawOnScreen(callback.getShapeIdx(), tan(angle) / maxCordX, 0.0f, scale.first, scale.second);
}

void Camera::sendRay(const MyWorld &world, MyCallback &rayCallback, const b2Vec2 &cameraPosition, const b2Vec2 &ray)
{
    // Changing direction of vector in callback
    rayCallback.ray_ = -ray;

    // Casting a Box2D ray
    world.RayCast(&rayCallback, cameraPosition, cameraPosition + ray);
}

void Camera::sendRay(const MyWorld &world, MyCallback &rayCallback, const b2Vec2 &cameraPosition, float angle)
{
    // Create calculate ray
    b2Vec2 ray{rayLength_ * getVector(angle)};

    // Send ray in a given direction
    sendRay(world, rayCallback, cameraPosition, ray);
    // world.RayCast(&rayCallback, begin, begin + ray);
}

bool Camera::ifInFieldOfView(const Object &camera, const Object &object)
{
    // Create view vector
    b2Vec2 viewVector{getVector(camera.getAngle())};

    // Create vector to given object
    b2Vec2 objectVector{getVector(camera.getPosition(), object.getPosition())};

    // Check if the angle between the two vectors is less than FOVMaxAngle
    float angle{vecAngle(viewVector, objectVector)};
    // std::cerr << angle << '\n';
    return angle < FOVMaxAngle_;
}

void Camera::drawViewOnScreen(UserIO &userIO, const MyWorld &world, const Object &camera, const Object2D &object2D)
{
    userIO.start();

    // Angle between casted rays
    constexpr float angleChange{2.0f * FOVMaxAngle_ / raysNumber_};

    // Cast a rays in many directions for Object3Ds
    for (float angle{-FOVMaxAngle_}; angle <= FOVMaxAngle_; angle += angleChange)
    {
        MyCallback rayCallback;

        // Rays can only hit Object3Ds
        sendRay(world, rayCallback, camera.getPosition(), angle + camera.getAngle());

        // Draw if the ray went shorter than max distance
        if (rayCallback.getFraction() < maxFraction_)
            drawRay(userIO, angle, rayCallback);
    }

    // Get all Object2Ds
    // For each one check if it is in current fieldOfView
    if (ifInFieldOfView(camera, object2D))
    {
        // If yes - check if nothing blocks its view
        //          by sending a ray in this direction
        MyCallback rayCallback;
        b2Vec2 ray{getVector(camera.getPosition(), object2D.getPosition())};
        sendRay(world, rayCallback, camera.getPosition(), ray);

        if (rayCallback.getFraction() == maxFraction_)
        {
            // If no  - draw it
            rayCallback.shapeIdx_ = object2D.getShapeIdx();
            float objectAngle{vecAngle(getVector(camera.getAngle()), ray)};
            float cross = b2Cross(getVector(camera.getAngle()), ray);
            objectAngle *= abs(cross) / cross;
            drawRay(userIO, objectAngle, rayCallback, b2Distance(getVector(camera.getAngle()), ray));
        }
    }

    userIO.end();
}

float Camera::MyCallback::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction)
{
    MyBody *ptr = dynamic_cast<MyBody *>(fixture->GetBody());
    if (!ptr)
        return 1.0f;
    shapeIdx_ = ptr->getObject().getShapeIdx();
    normal_ = normal;
    return fraction_ = fraction;
    (void)point; // For -Werror=unused-variable
}
