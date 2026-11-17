#pragma once

#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "UFO.h"

using namespace sf;
using namespace sfp;

class UFOController
{
private:
    World& world;
    Vector2u windowSize;
    UFO& ufo;
    std::function<void()> onUFOMoveCallback;

    long spawnRate;
    long spawnTimer = 0;
    float movementSpeedMultiplier;

    int direction = 1;

public:
    /**
     * @brief Initializes the UFO controller.
     *
     * @param world Reference to the physics world.
     * @param windowSize Dimensions of the game window.
     * @param ufo Reference to the UFO entity.
     * @param spawnRate Time between UFO spawn attempts in milliseconds.
     * @param movementSpeedMultiplier Multiplier applied to the UFO's movement speed.
     */
    UFOController(World& world, const Vector2u& windowSize, UFO& ufo, long spawnRate = 25000, float movementSpeedMultiplier = 0.5)
        : world(world), windowSize(windowSize), ufo(ufo), spawnRate(spawnRate), movementSpeedMultiplier(movementSpeedMultiplier)
    {
        ufo.setStatic(true);
        ufo.setColor(Color(255, 0, 0, 255));
    }

    /**
     * @brief Sets the callback triggered when the UFO moves.
     *
     * @param callback The callback to use.
     */
    void setOnUFOMoveCallback(auto callback)
    {
        onUFOMoveCallback = callback;
    }

    /**
     * @brief Spawns a UFO.
     */
    void spawnUFO()
    {
        ufo.setCenter(Vector2f(-80.0f, 320.0f));
        ufo.setActive(true);
        world.AddPhysicsBody(ufo);
    }

    /**
     * @brief Removes the UFO.
     */
    void removeUFO()
    {
        ufo.setActive(false);
        world.RemovePhysicsBody(ufo);
    }

    /**
     * @brief Updates the UFO.
     * 
     * @param deltaMS The elapsed time in milliseconds.
     */
    void update(long deltaTime)
    {
        if (!ufo.isActive())
        {
            // Return if the timer hasn't been met
            spawnTimer += deltaTime;
            if (spawnTimer < spawnRate) return;

            // Set the timer back to 0 on successful spawn
            spawnTimer = 0;

            spawnUFO();
            return;
        }

        Vector2f position = ufo.getCenter();
        position.x += movementSpeedMultiplier * deltaTime;
        ufo.setCenter(position);

        onUFOMoveCallback();

        // Check if UFO has left the screen
        if (position.x - 80.0f > windowSize.x) removeUFO();
    }
};