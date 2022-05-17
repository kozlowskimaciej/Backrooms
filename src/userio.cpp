#include "userio.h"

sf::Color UserIO::dimColor(const sf::Color &color, float dimFactor)
{
    dimFactor = std::max(0.0f, std::min(dimFactor, 1.0f));
    return sf::Color(color.r * dimFactor,
                     color.g * dimFactor,
                     color.b * dimFactor,
                     color.a);
}

void UserIO::drawOnScreen(Shapes::Type shapeIdx, float x, float y, float xScale, float yScale, float dim, float textureOffset, bool shader)
{
    sf::Vector2u size{window_.getSize()};
    sf::RectangleShape shape{Shapes::getShape(shapeIdx, textureOffset)};
    const sf::Color &originalColor = shape.getFillColor();
    shape.setFillColor(std::move(dimColor(originalColor, dim)));
    shape.scale(xScale, yScale);

    shape.setPosition(size.x / 2.0f * (x + 1.0f), size.y / 2.0f * (y + 1.0f));

    if (shader)
    {
        shader_.setUniform("renderDist", Conf::renderDistance);
        window_.draw(shape, &shader_);
    }
    else
        window_.draw(shape);
}

bool UserIO::handleKeyPress(sf::Keyboard::Key key)
{
    bool temp = keyPresses_[key];
    keyPresses_[key] = false;
    return temp;
}

void UserIO::handleEvents()
{
    sf::Event event;
    while (window_.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window_.close();
        if (event.type == sf::Event::LostFocus)
            focus_ = false;
        if (event.type == sf::Event::GainedFocus)
        {
            focus_ = true;
            getMouseXMovement(); // Reseting mouse position
        }
        if (event.type == sf::Event::KeyPressed)
        {
            keyPresses_[event.key.code] = true;
        }
    }
}

int UserIO::getMouseXMovement()
{
    if (!focus_) // Don't turn around if user is not focused on the window
        return 0;
    static const sf::Vector2i defaultPosition(100, 100);
    int mouseMovement{sf::Mouse::getPosition(window_).x - defaultPosition.x};
    sf::Mouse::setPosition(defaultPosition, window_);
    return mouseMovement;
}
