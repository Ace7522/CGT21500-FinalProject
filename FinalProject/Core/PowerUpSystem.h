#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "../Game/PowerUp.h"
#include "GameFlags.h"

using namespace sf;
using namespace sfp;

class PowerUpSystem
{
private:
    World& world;
    Vector2u windowSize;
    std::vector<PowerUp> powerUps;
    std::mt19937 generator{ std::random_device{}() };

    std::uint8_t activePowerUps = 0;
    long speedPowerUpDuration = 0;
    long multiShotPowerUpDuration = 0;
    long rapidFirePowerUpDuration = 0;

    long spawnTimer = 0;
    long spawnRate = 7500;

public:
    /**
     * @brief Creates a power up system.
     *
     * @param world Reference to the physics world.
     * @param windowSize Size of the game window.
     */
    PowerUpSystem(World& world, Vector2u windowSize)
        : world(world), windowSize(windowSize), powerUps(5)
    {
        initializePowerUps();
    }

private:
    /**
     * @brief Initializes all power ups by placing them off screen.
     */
    void initializePowerUps()
    {
        for (PowerUp& powerUp : powerUps)
        {
            powerUp.setCenter(Vector2f(-10, -10));
            powerUp.setActive(false);

            world.AddPhysicsBody(powerUp);
        }
    }

    /**
     * @brief Spawns a power up.
     */
    void spawnPowerUp()
    {
        // Random power up type
        std::uniform_int_distribution<int> typeDistribution(0, 2);

        // Random position
        std::uniform_int_distribution<int> xDistribution(
            50, static_cast<int>(windowSize.x) - 50
        );
        std::uniform_int_distribution<int> yDistribution(
            static_cast<int>(windowSize.y) - 450, static_cast<int>(windowSize.y) - 250
        );

        for (PowerUp& powerUp : powerUps)
        {
            if (!powerUp.isActive())
            {
                // Choose a random type
                std::uint8_t type = PowerUpTypes::SPEED;

                switch (typeDistribution(generator))
                {
                    case 0: type = PowerUpTypes::SPEED; break;
                    case 1: type = PowerUpTypes::MULTISHOT; break;
                    case 2: type = PowerUpTypes::RAPIDFIRE; break;
                }

                // Choose a random position
                Vector2f position(
                    static_cast<float>(xDistribution(generator)),
                    static_cast<float>(yDistribution(generator))
                );

                powerUp.setType(type);
                powerUp.setCenter(position);
                powerUp.setActive(true);
                world.AddPhysicsBody(powerUp);

                return;
            }
        }
    }

    /**
     * @brief Checks if enough time has passed since the last power up.
     * 
     * @param deltaMS The elapsed time in milliseconds.
     */
    void updateSpawningPowerUps(long deltaMS)
    {
        spawnTimer += deltaMS;

        if (spawnTimer >= spawnRate)
        {
            spawnPowerUp();
            spawnTimer = 0;
        }
    }

    /**
     * @brief Updates the power up durations.
     *
     * @param deltaMS The elapsed time in milliseconds.
     */
    void updatePowerUps(long deltaMS)
    {
        if (speedPowerUpDuration > 0)
        {
            speedPowerUpDuration -= deltaMS;

            if (speedPowerUpDuration <= 0)
            {
                speedPowerUpDuration = 0;
                activePowerUps &= ~PowerUpTypes::SPEED;
            }
        }

        if (multiShotPowerUpDuration > 0)
        {
            multiShotPowerUpDuration -= deltaMS;

            if (multiShotPowerUpDuration <= 0)
            {
                multiShotPowerUpDuration = 0;
                activePowerUps &= ~PowerUpTypes::MULTISHOT;
            }
        }

        if (rapidFirePowerUpDuration > 0)
        {
            rapidFirePowerUpDuration -= deltaMS;

            if (rapidFirePowerUpDuration <= 0)
            {
                rapidFirePowerUpDuration = 0;
                activePowerUps &= ~PowerUpTypes::RAPIDFIRE;
            }
        }
    }

public:
    /**
     * @brief Activates a power up with a given duration.
     *
     * @param powerUp The power up type.
     * @param durationMS The duration the power up should last in milliseconds.
     */
    void activatePowerUp(std::uint8_t powerUp, long durationMS = 5000)
    {
        activePowerUps |= powerUp;

        switch (powerUp)
        {
            case PowerUpTypes::SPEED: speedPowerUpDuration = durationMS; break;
            case PowerUpTypes::MULTISHOT: multiShotPowerUpDuration = durationMS; break;
            case PowerUpTypes::RAPIDFIRE:  rapidFirePowerUpDuration = durationMS; break;
        }
    }

    /**
     * @brief Returns the active states of all power ups as an unsigned integer.
     *
     * @return The active power ups.
     */
    std::uint8_t getActivePowerUps() const
    {
        return activePowerUps;
    }

    /**
     * @brief Returns a non const vector containing all the power ups.
     *
     * @return The non const vector of power ups.
     */
    std::vector<PowerUp>& getPowerUps()
    {
        return powerUps;
    }

    /**
     * @brief Returns a const vector containing all the power ups.
     *
     * @return The const vector of power ups.
     */
    const std::vector<PowerUp>& getPowerUps() const
    {
        return powerUps;
    }

    /**
     * @brief Removes a power up from the game.
     * 
     * @param powerUp The power up to remove.
     */
    void removePowerUp(PowerUp& powerUp)
    {
        powerUp.setActive(false);
        powerUp.setCenter(Vector2f(-10, -10));
        world.RemovePhysicsBody(powerUp);
    }

    /**
     * @brief Updates the power up duration and spawn timers.
     *
     * @param deltaMS The elapsed time in milliseconds
     */
    void update(long deltaMS)
    {
        updatePowerUps(deltaMS);
        updateSpawningPowerUps(deltaMS);
    }
};