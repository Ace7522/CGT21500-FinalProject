#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace sf;
using namespace sfp;

struct Wall
{
    PhysicsRectangle rectangle;
    Vector2f size;
    Vector2f center;
};

/**
 * @brief Generates the boundaries of the game.
 *
 * @param window The render window.
 * @param insetX How far to inset the top and bottom boundaries relative to the edge of the game screen.
 * @param insetY How far to inset the left and right boundaries relative to the edge of the game screen.
 * 
 * @return A vector containing all the Wall structs.
 */
std::vector<Wall> generateBounds(RenderWindow& window, int insetX = 0, int insetY = 0)
{
    const Vector2u SIZE = window.getSize();
    const int WIDTH = SIZE.x;
    const int HEIGHT = SIZE.y;
    const int THICKNESS = 5;

    std::vector<Wall> walls = {
        { PhysicsRectangle(), Vector2f(WIDTH, THICKNESS), Vector2f(WIDTH / 2.0f, insetY) }, // Top
        { PhysicsRectangle(), Vector2f(WIDTH, THICKNESS), Vector2f(WIDTH / 2.0f, HEIGHT - insetY) }, // Bottom
        { PhysicsRectangle(), Vector2f(THICKNESS, HEIGHT - 2 * insetY - THICKNESS), Vector2f(insetX, HEIGHT / 2.0f) }, // Left
        { PhysicsRectangle(), Vector2f(THICKNESS, HEIGHT - 2 * insetY - THICKNESS), Vector2f(WIDTH - insetX, HEIGHT / 2.0f) } // Right
    };

    for (Wall& wall : walls)
    {
        wall.rectangle.setSize(wall.size);
        wall.rectangle.setCenter(wall.center);
        wall.rectangle.setStatic(true);
    }

    return walls;
}