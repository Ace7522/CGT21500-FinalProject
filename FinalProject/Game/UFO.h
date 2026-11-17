#pragma once

#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "../Utils/SFUtils.h"

using namespace sf;
using namespace sfp;

class UFO : public PhysicsSprite
{
private:
    Texture texture;
    bool active = false;

public:
    /**
     * @brief Creates a UFO.
     */
    UFO()
    {
        LoadTex(texture, "Assets/Images/ufo.png");
        setTexture(texture);
        setSize(Vector2f(texture.getSize().x, texture.getSize().y));
    }

private:
    // Score values for the ufo taken from the game, player shots increment through this array
    const int scoreValues[15] = {
        50, 50, 100, 150, 100,
        100, 50, 300, 100, 100,
        100, 50, 150, 100, 100
    };
    int scoreIndex = 0;

public:
    /**
     * @brief Checks if the enemy is active.
     *
     * @return True if active, otherwise false.
     */
    bool isActive() const
    {
        return active;
    }

    /**
     * @brief Sets a projectiles active state.
     *
     * @param state The new state.
     */
    void setActive(bool state)
    {
        active = state;
    }

    /**
     * @brief Increments the index of the score selector by one.
     */
    void incrementScoreIndex()
    {
        scoreIndex++;
        if (scoreIndex >= 15) scoreIndex = 0;
    }

    /**
     * @brief Gets the score value associated with the ufo.
     *
     * @return The score value.
     */
    int getScoreValue() const
    {
        return scoreValues[scoreIndex];
    }
};