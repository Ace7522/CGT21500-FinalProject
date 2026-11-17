#pragma once

#include <iostream>
#include <functional>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "Enemy.h"
#include "UFO.h"

using namespace sf;
using namespace sfp;

class EnemyController
{
private:
    Vector2u windowSize;
    std::mt19937 generator{ std::random_device{}() };

    long fireRate;
    long fireTimer = 0;
    std::function<void(const Vector2f&, const Vector2f&)> onEnemyFireCallback;
    Vector2f fireVelocity;

    long movementTimer = 0;
    long movementRate;
    std::function<void()> onEnemyMoveCallback;

    float movementDistance = 20.0f;
    float dropDistance = 80.0f;

    bool movingRight = true;
    bool moveDown = false;
    Vector2f furthestAdvancedEnemyPosition = Vector2f(0.0f, 0.0f);

public:
    /**
     * @brief Initializes the enemy controller.
     *
     * @param windowSize Size of the game window.
     * @param movementRate Time between enemy movement updates in milliseconds.
     * @param fireRate Time between enemy firing intervals in milliseconds.
     * @param fireVelocity Velocity of enemy projectiles.
     */
    EnemyController(Vector2u windowSize, long movementRate = 800, long fireRate = 1000, Vector2f fireVelocity = Vector2f(0, 1))
        : windowSize(windowSize), movementRate(movementRate), fireRate(fireRate), fireVelocity(fireVelocity)
    {
    }

private:
    /**
     * @brief Updates the movement of the enemies.
     *
     * @param enemies The vector of enemies.
     * @param deltaMS The elapsed time in milliseconds.
     */
    void updateMovement(std::vector<Enemy>& enemies, long deltaMS)
    {
        // Return if the timer hasn't been met, or the enemies vector is empty
        movementTimer += deltaMS;
        if (movementTimer < movementRate) return;

        // Set the timer back to 0 on successful move
        movementTimer = 0;

        float leftmost = windowSize.x - 10.0f;
        float rightmost = 10.0f;

        // Find the leftmost and rightmost enemies
        for (Enemy& enemy : enemies)
        {
            if (!enemy.isActive()) continue;

            FloatRect enemyBounds = enemy.getGlobalBounds();

            float left = enemyBounds.left;
            float right = enemyBounds.left + enemyBounds.width;

            if (left < leftmost) leftmost = left;
            if (right > rightmost) rightmost = right;
        }

        // Check whether the formation has reached an edge
        if (movingRight && (rightmost + movementDistance >= windowSize.x))
        {
            movingRight = false;
            moveDown = true;
        }
        else if (!movingRight && (leftmost - movementDistance <= 0))
        {
            movingRight = true;
            moveDown = true;
        }

        // Move all enemies
        for (Enemy& enemy : enemies)
        {
            if (!enemy.isActive() || enemy.getIsDying()) continue;

            Vector2f position = enemy.getCenter();
            position.x += movingRight ? movementDistance : -movementDistance;
            if (moveDown) position.y += dropDistance;

            enemy.setCenter(position);
            enemy.update(deltaMS);
        }

        moveDown = false;
        onEnemyMoveCallback();
    }

    /**
     * @brief Returns a random enemy pointer.
     *
     * @param enemies The vector of enemies.
     * 
     * @return The pointer to a randomly selected enemy.
     */
    Enemy* getRandomEnemy(std::vector<Enemy>& enemies)
    {
        // Create a vector for storing enemy pointers for enemies allowed to shoot
        std::vector<Enemy*> validEnemies;

        // Iterate through all the enemies
        for (Enemy& enemy : enemies)
        {
            // Skip non active enemies
            if (!enemy.isActive()) continue;
            bool found = false;

            // Iterate through existing shooter pointers by reference
            for (Enemy*& validEnemy : validEnemies)
            {
                // Check whether an enemy from this column is already registered
                if (validEnemy->getCol() == enemy.getCol())
                {
                    found = true;

                    // Replace the existing shooter with the current enemy
                    validEnemy = &enemy;
                    break;
                }
            }

            if (!found) validEnemies.push_back(&enemy);
        }

        // Return a null ptr if no enemies are valid
        if (validEnemies.empty()) return nullptr;

        // Randomly select an eligible shooter
        std::uniform_int_distribution<int> distribution(
            0, static_cast<int>(validEnemies.size()) - 1
        );

        return validEnemies[distribution(generator)];
    }

    /**
     * @brief Updates the furthest advanced position.
     *
     * @param enemies The vector of enemies.
     */
    void updateFurthestAdvancedEnemyPosition(std::vector<Enemy>& enemies)
    {
        for (Enemy& enemy : enemies)
        {
            if (!enemy.isActive()) continue;
            if (enemy.getCenter().y > furthestAdvancedEnemyPosition.y) furthestAdvancedEnemyPosition = enemy.getCenter();
        }
    }

    /**
     * @brief Processes enemy attacks.
     * 
     * @param enemies The vector of enemies.
     * @param deltaMS The elapsed time in milliseconds.
     */
    void processAttack(std::vector<Enemy>& enemies, long deltaMS)
    {
        // Return if the timer hasn't been met
        fireTimer += deltaMS;
        if (fireTimer < fireRate) return;

        // Set the timer back to 0 on successful attack
        fireTimer = 0;
        
        Enemy* randomEnemy = getRandomEnemy(enemies);
        if (randomEnemy == nullptr) return;

        // Vector2f(x, y) is the offset from the enemy
        onEnemyFireCallback(randomEnemy->getCenter() + Vector2f(0.0f, 60.0f), fireVelocity);
    }

public:
    /**
     * @brief Sets the callback triggered when the enemy fires.
     *
     * @param callback The callback to use.
     */
    void setOnEnemyFireCallback(auto callback)
    {
        onEnemyFireCallback = callback;
    }

    /**
     * @brief Sets the callback triggered when the enemies move.
     *
     * @param callback The callback to use.
     */
    void setOnEnemyMoveCallback(auto callback)
    {
        onEnemyMoveCallback = callback;
    }

    /**
     * @brief Decrements the movement interval used by the controller.
     *
     * @param amount The amount to decrease by.
     */
    void decrementMovementInterval(long amount)
    {
        movementRate -= amount;
        if (movementRate < 20) movementRate = 20;
    }

    /**
     * @brief Gets the y position of the furthest advanced enemy.
     *
     * @return The y position.
     */
    float getFurthestAdvancedEnemyPosition() const
    {
        return furthestAdvancedEnemyPosition.y;
    }

    /**
     * @brief Updates all the enemies.
     *
     * @param enemies The vector of enemies.
     * @param deltaMS The elapsed time in milliseconds.
     */
    void update(std::vector<Enemy>& enemies, long deltaMS)
    {
        updateMovement(enemies, deltaMS);
        updateFurthestAdvancedEnemyPosition(enemies);
        processAttack(enemies, deltaMS);

        for (Enemy& enemy : enemies)
        {
            if (enemy.isActive()) enemy.updateDying(deltaMS);
        }
    }
};