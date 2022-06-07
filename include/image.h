#ifndef IMAGE_5262413_H
#define IMAGE_5262413_H

#include <SFML/Graphics.hpp> // for using sf::Shape
#include "textures.h"        // for setting sf::Texture
#include "config.h"          // for window size
#include "timer.h"

class Image
{
public:
    enum Type
    {
        WALL,
        RED_WALL,
        ENEMY,
        EMF,
        EMF1,
        EMF2,
        EMF3,
        EMF_DROPPED,
        FLASHLIGHT,
        BACKGROUND,
        PAGE,
        NOISE,
        EMPTY,
        TOTAL
    };

    static sf::RectangleShape getShape(Type shapeIdx, float horOffset = 0.0f);

    // Adds textures to some shapes
    static void init();

private:
    static Image allImages_[Type::TOTAL];

    unsigned int frameCount_{1u};
    unsigned int currentFrame_{0u};
    const double animationPeriod_{0.5};
    sf::RectangleShape shape_;
    Timer animationTimer_;

    const sf::Texture *getTexture() const { return shape_.getTexture(); }
    const sf::Vector2f &getSize() const { return shape_.getSize(); }
    static void scaleBasedOnScreen(sf::RectangleShape &shape);
    static void makeFullScreen(sf::RectangleShape &shape);

    void addShape(float width, float height, bool fullScreen = false, sf::Color color = sf::Color::White);
    void addAnimation(unsigned int frameCount);
    void addTexture(Textures::Type type);
};

#endif
