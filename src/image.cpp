#include "image.h"

Image Image::allImages_[Type::TOTAL]{};

sf::RectangleShape Image::getShape(Type shapeIdx, float horOffset)
{
    Image &image{allImages_[shapeIdx]};

    // Go to next frame after time has passed
    if (image.animationTimer_.elapsed() > image.animationPeriod_)
    {
        // Reset timer
        image.animationTimer_.reset();

        // Go to next frame
        ++image.currentFrame_ %= image.frameCount_;
        // image.currentFrame_ = (image.currentFrame_ + 1) % image.frameCount_;
    }

    assert(shapeIdx >= 0 && shapeIdx < TOTAL);
    assert(image.currentFrame_ < image.frameCount_);

    if (image.getTexture())
    {
        constexpr float offsetFactor{10.0f};

        sf::Vector2f shapeSize = image.getSize();
        sf::Vector2u textureSize = image.getTexture()->getSize();
        unsigned int offsetMod{static_cast<int>(shapeSize.x * horOffset * offsetFactor) % textureSize.x};

        // std::cerr << image.currentFrame_ / image.frameCount_ * textureSize.y << '\n';

        image.shape_.setTextureRect(sf::IntRect(
            sf::Vector2i(offsetMod, image.currentFrame_ * textureSize.y / image.frameCount_),
            sf::Vector2i(shapeSize.x, textureSize.y / image.frameCount_)));
    }
    return image.shape_;
}

void Image::init()
{
    allImages_[WALL].addShape(2.0f, 50.0f);
    allImages_[WALL].addTexture(Textures::WALL);

    allImages_[RED_WALL].addShape(2.0f, 50.0f, false, sf::Color::Red);

    allImages_[ENEMY].addShape(509.0f, 798.0f);
    allImages_[ENEMY].addTexture(Textures::ENEMY);
    // allImages_[ENEMY].addAnimation(3u);

    allImages_[EMF].addShape(1864.0f, 1051.0f, true);
    allImages_[EMF].addTexture(Textures::EMF);

    allImages_[EMF1].addShape(1864.0f, 1051.0f, true);
    allImages_[EMF1].addTexture(Textures::EMF1);

    allImages_[EMF2].addShape(1864.0f, 1051.0f, true);
    allImages_[EMF2].addTexture(Textures::EMF2);

    allImages_[EMF3].addShape(1864.0f, 1051.0f, true);
    allImages_[EMF3].addTexture(Textures::EMF3);

    allImages_[EMF_DROPPED].addShape(134.0f, 452.0f);
    allImages_[EMF_DROPPED].addTexture(Textures::EMF_DROPPED);

    allImages_[FLASHLIGHT].addShape(1920.0f, 1080.0f, true);
    allImages_[FLASHLIGHT].addTexture(Textures::FLASHLIGHT);

    allImages_[BACKGROUND].addShape(1777.0f, 1000.0f, true);
    allImages_[BACKGROUND].addTexture(Textures::BACKGROUND);

    allImages_[PAGE].addShape(200.f, 283.f);
    allImages_[PAGE].addTexture(Textures::PAGE);

    allImages_[NOISE].addShape(10.f, 10.f, true);
}

void Image::scaleBasedOnScreen(sf::RectangleShape &shape)
{
    float xScaleFactor{Conf::windowWidth / 1000.f};
    shape.scale(xScaleFactor, 1.0f);
}

void Image::makeFullScreen(sf::RectangleShape &shape)
{
    sf::Vector2f shapeSize{shape.getSize()};
    shape.setScale(Conf::windowWidth / shapeSize.x, Conf::windowHeight / shapeSize.y);
}

void Image::addShape(float width, float height, bool fullScreen, sf::Color color)
{
    sf::Vector2f size(width, height);
    shape_ = sf::RectangleShape(size);
    shape_.setOrigin(size * 0.5f); // Sets origin to shape's center
    shape_.setFillColor(color);
    scaleBasedOnScreen(shape_);
    if (fullScreen)
        makeFullScreen(shape_);
}

void Image::addAnimation(unsigned int frameCount)
{
    frameCount_ = frameCount;
}

void Image::addTexture(Textures::Type type)
{
    shape_.setTexture(&Textures::getTexture(type));
}
