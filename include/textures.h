#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics.hpp> // for using sf::Texture
#include <cassert>           // for checking texture type

class Textures
{
public:
    enum Type
    {
        ENEMY,
        WALL,
        EMF,
        EMF1,
        EMF2,
        EMF3,
        EMF_DROPPED,
        FLASHLIGHT,
        BACKGROUND,
        PAGE,
        TOTAL
    };
    static bool init();
    static const sf::Texture &getTexture(Type textureType);

private:
    static sf::Texture textures_[TOTAL];
};

#endif
