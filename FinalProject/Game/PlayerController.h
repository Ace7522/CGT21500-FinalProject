#pragma once

#include <iostream>
#include <cstdint>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "../Core/GameFlags.h"
#include "GenerateBounds.h"
#include "KeyboardInput.h"
#include "Player.h"

using namespace sf;
using namespace sfp;

class PlayerController
{
private:
    float bottomBound;
    float leftBound;
    float rightBound;

	long fireRate;
    long fireTimer = 0;
    Vector2f fireVelocity;
	float movementSpeedMultiplier;
    std::function<void(const Vector2f&, const Vector2f&)> onPlayerFireCallback;
    std::function<std::uint8_t()> checkPowerUpsCallback;

public:
    /**
     * @brief Initializes the player controller.
     *
     * @param walls The bounding walls of the game.
     * @param fireRate Time between player firing intervals in milliseconds.
     * @param fireVelocity Velocity of player projectiles.
     * @param movementSpeedMultiplier Multiplier applied to the player's movement speed.
     */
	PlayerController(std::vector<Wall>& walls, long fireRate = 1200, Vector2f fireVelocity = Vector2f(0, -2), float movementSpeedMultiplier = 0.4)
		: bottomBound(walls[1].center.y - walls[1].size.y / 2), leftBound(walls[2].center.x + walls[2].size.x / 2), rightBound(walls[3].center.x - walls[3].size.x / 2),
            fireRate(fireRate), fireVelocity(fireVelocity), movementSpeedMultiplier(movementSpeedMultiplier)
	{
	}

private:
    /**
     * @brief Processes player movement.
     *
     * @param player The player.
     * @param input An unsigned integer representing the pressed keyboard keys.
     * @param deltaMS The elapsed time in milliseconds.
     */
    void processMovement(Player& player, std::uint8_t input, long deltaMS) const
    {
        Vector2f velocity(0, 0);

        // Adjust velocity values based on input
        if (input & KeyboardInputTypes::W_KEY) velocity.y -= 1;
        if (input & KeyboardInputTypes::S_KEY) velocity.y += 1;
        if (input & KeyboardInputTypes::A_KEY) velocity.x -= 1;
        if (input & KeyboardInputTypes::D_KEY) velocity.x += 1;

        // Normalize the velocity
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (length > 0) velocity /= length;

        // Multiply the normalized velocity by the speed multiplier, if a speed power up is active multiply by 2
        float speedPowerUpMultiplier = checkPowerUpsCallback() & PowerUpTypes::SPEED ? 2.0f : 1.0f;

        // Calculate the player's new position
        Vector2f position = player.getCenter();
        position += velocity * movementSpeedMultiplier * speedPowerUpMultiplier * static_cast<float>(deltaMS);

        // Clamp the player inside a box
        const int Y_LEVEL_CLAMP = 1585;
        if (position.y < Y_LEVEL_CLAMP) position.y = Y_LEVEL_CLAMP;
        if (position.y > bottomBound) position.y = bottomBound;
        if (position.x < leftBound + 50) position.x = leftBound + 50;
        if (position.x > rightBound - 50) position.x = rightBound - 50;

        player.setCenter(position);
    }

    /**
     * @brief Processes player attacks.
     * 
     * @param player The player.
     * @param input An unsigned integer representing the pressed keyboard keys.
     * @param deltaMS The elapsed time in milliseconds.
     */
    void processAttack(Player& player, std::uint8_t input, long deltaMS)
    {
        // Return if the timer hasn't been met
        fireTimer += deltaMS;
        if (!(input & KeyboardInputTypes::E_KEY)) return;
        if (fireTimer < fireRate * (checkPowerUpsCallback() & PowerUpTypes::RAPIDFIRE ? 0.25f : 1.0f)) return;

        // Set the timer back to 0 on successful attack
        fireTimer = 0;

        // Vector2f(x, y) is the offset from the player
        onPlayerFireCallback(player.getCenter() + Vector2f(0, -50), fireVelocity);

        if (checkPowerUpsCallback() & PowerUpTypes::MULTISHOT)
        {
            onPlayerFireCallback(player.getCenter() + Vector2f(0, -50), Vector2f(0.5f, fireVelocity.y));
            onPlayerFireCallback(player.getCenter() + Vector2f(0, -50), Vector2f(-0.5f, fireVelocity.y));
        }
    }

public:
    /**
     * @brief Sets the callback triggered when the player fires.
     *
     * @param callback The callback to use.
     */
    void setOnPlayerFireCallback(auto callback)
    {
        onPlayerFireCallback = callback;
    }

    /**
     * @brief Sets the callback for checking powerups.
     *
     * @param callback The callback to use.
     */
    void setCheckPowerUpsCallback(auto callback)
    {
        checkPowerUpsCallback = callback;
    }

    /**
     * @brief Sets the movement speed multiplier.
     *
     * @param value The new value.
     */
    void setMovementSpeedMultiplier(int value)
    {
        movementSpeedMultiplier = value;
    }

    /**
     * @brief Processes all the inputs.
     * 
     * @param player The player.
     * @param input An unsigned integer representing the pressed keyboard keys.
     * @param deltaMS The elapsed time in milliseconds.
     */
    void processInput(Player& player, std::uint8_t input, long deltaMS)
    {
        processMovement(player, input, deltaMS);
        processAttack(player, input, deltaMS);
    }

    /**
     * @brief Updates the player's hit animation.
     *
     * @param player The player.
     * @param deltaMS The elapsed time in milliseconds.
     * 
     * @return True if the player is hit, otherwise false.
     */
    bool updateHit(Player& player, long deltaMS)
    {
        player.updateHit(deltaMS);

        return player.getHit();
    }

    /**
     * @brief Updates the player.
     *
     * @param player The player.
     * @param deltaMS The elapsed time in milliseconds.
     */
    void update(Player& player, long deltaMS)
    {
        player.update(deltaMS);
    }
};