#include "textures.h"

sf::Texture Textures::textures_[TOTAL]{};

const sf::Texture &Textures::getTexture(Type textureType)
{
    assert(textureType >= 0 && textureType < TOTAL);

    return textures_[textureType];
}

bool Textures::init()
{
    bool success{true};
    success = success && textures_[ENEMY].loadFromFile("media/enemy.png");
    success = success && textures_[WALL].loadFromFile("media/concrete_texture.jpg");
    success = success && textures_[EMF].loadFromFile("media/emf/emf.png");
    success = success && textures_[EMF1].loadFromFile("media/emf/emf1.png");
    success = success && textures_[EMF2].loadFromFile("media/emf/emf2.png");
    success = success && textures_[EMF3].loadFromFile("media/emf/emf3.png");
    success = success && textures_[EMF_DROPPED].loadFromFile("media/emf/emf_dropped.png");
    success = success && textures_[FLASHLIGHT].loadFromFile("media/flashlight.png");
    success = success && textures_[BACKGROUND].loadFromFile("media/background.png");
    success = success && textures_[PAGE].loadFromFile("media/page.png");
    return success;
}
