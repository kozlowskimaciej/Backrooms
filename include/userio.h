#ifndef USER_IO_H
#define USER_IO_H

#include "config.h"
#include "image.h"           // for drawing sf::Shape
#include <SFML/Graphics.hpp> // for using sf::RenderWindow
#include <memory>            // for moving sf::Color
#include <algorithm>         // for std::min
#include <iostream>

class UserIO
{
private:
    sf::RenderWindow window_;
    bool focus_{true}; // Focus on game window
    std::map<sf::Keyboard::Key, bool> keyPresses_;

    sf::Color dimColor(const sf::Color &color, float dimFactor);

public:
    UserIO(const std::string &title);

    // The 0, 0 coordinate coresponds to the center of the screen
    void drawOnScreen(Image::Type shapeIdx, float x = 0, float y = 0, float xScale = 1.0f, float yScale = 1.0f, float dim = 1.0f, float textureOffset = 0.0);

    bool isOpen() { return window_.isOpen(); }

    // Starts making new frame
    void start();

    // Displays frame
    void end();
    bool handleKeyPress(sf::Keyboard::Key key);
    void handleEvents();
    int getMouseXMovement();
};

#endif
