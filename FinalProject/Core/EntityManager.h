#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "GameStateSystem.h"
#include "ProjectileSystem.h"
#include "SoundSystem.h"
#include "PowerUpSystem.h"
#include "../Game/GenerateBounds.h"
#include "../Game/Player.h"
#include "../Game/PlayerController.h"
#include "../Game/Enemy.h"
#include "../Game/EnemyController.h"
#include "../Game/UFO.h"
#include "../Game/UFOController.h"
#include "../Game/Projectile.h"

using namespace sf;
using namespace sfp;

class EntityManager
{
private:
    World& world;
    ProjectileSystem& projectileSystem;
    SoundSystem& soundSystem;
    PowerUpSystem& powerUpSystem;
    Player player;
    PlayerController playerController;
    std::vector<Enemy> enemies;
    EnemyController enemyController;
    UFO ufo;
    UFOController ufoController;
    std::function<State()> checkGameStateCallback;
    std::function<void()> setGameStatePlayingCallback;

public:
    /**
     * @brief Initializes the entity manager and its game entities and controllers.
     *
     * @param world Reference to the physics world.
     * @param windowSize Size of the game window.
     * @param walls The bounding walls of the game.
     * @param projectileSys Reference to the projectile system.
     * @param SoundSys Reference to the sound system.
     * @param powerUpSys Reference to the power up system.
     * @param checkGameStateCallback A callback for checking the game state.
     * @param setGameStatePlayingCallback A callback for setting the game to playing.
     */
    EntityManager(
        World& world, const Vector2u& windowSize, std::vector<Wall>& walls,
        ProjectileSystem& projectileSys, SoundSystem& SoundSys, PowerUpSystem& powerUpSys,
        auto checkGameStateCallback, auto setGameStatePlayingCallback
    )
        : world(world), projectileSystem(projectileSys), soundSystem(SoundSys), powerUpSystem(powerUpSys), 
            player(windowSize), playerController(walls), enemyController(windowSize), ufo(), ufoController(world, windowSize, ufo),
            checkGameStateCallback(checkGameStateCallback), setGameStatePlayingCallback(setGameStatePlayingCallback)
    {
        // Gives the player controller a callback to the various systems on fire
        playerController.setOnPlayerFireCallback(
            [this](const Vector2f& position, const Vector2f& velocity)
            {
                projectileSystem.spawnProjectile(position, velocity, ProjectileType::PLAYER);
                soundSystem.playSound(SoundTypes::PLAYER_SHOOT);
                getUFO().incrementScoreIndex();
            }
        );

        // Gives the player controller a callback to the power up system
        playerController.setCheckPowerUpsCallback(
            [this]()
            {
                return powerUpSystem.getActivePowerUps();
            }
        );

        // Gives the player a callback to the input controller
        player.setInputCallback(
            [this](Player& player, std::uint8_t input, long deltaMS)
            {
                playerController.processInput(player, input, deltaMS);
            }
        );

        // Gives the enemy controller a callback to the various systems on fire
        enemyController.setOnEnemyFireCallback(
            [this](const Vector2f& position, const Vector2f& velocity)
            {
                projectileSystem.spawnProjectile(position, velocity, ProjectileType::ENEMY);
            }
        );

        // Gives the enemy a callback to the various systems on move
        enemyController.setOnEnemyMoveCallback(
            [this]()
            {
                soundSystem.playSound(SoundTypes::ENEMY_MOVEMENT);
            }
        );

        // Gives the ufo a callback to the various systems on move
        ufoController.setOnUFOMoveCallback(
            [this]()
            {
                soundSystem.playSound(SoundTypes::UFO_MOVEMENT);
            }
        );

        createEnemyFormation(windowSize);
    }

private:
    /**
     * @brief Spawns the base enemy formation.
     *
     * @param windowSize Size of the game window.
     */
    void createEnemyFormation(const Vector2u& windowSize)
    {
        // Based on the formation of actual space invaders
        const int ROWS = 5;
        const int COLUMNS = 11;

        // Get a sample enemy to retrieve the bounding box of an enemy
        Enemy sampleEnemy(EnemyType::OCTOPUS, 0, 0);
        float enemyWidth = sampleEnemy.getGlobalBounds().width;
        float enemyHeight = sampleEnemy.getGlobalBounds().height;

        // Spacing between enemies accounts for the size of an enemy
        const float SPACING_X = enemyWidth + 30.0f;
        const float SPACING_Y = enemyHeight + 30.0f;

        // Starting coord of the formation
        float startX = windowSize.x / 2.0f -  ((COLUMNS - 1) * SPACING_X) / 2.0f + enemyWidth / 2.0f - 150.0f;
        float startY = 450.0f;

        // Reserve space for all 55 enemies
        enemies.reserve(ROWS * COLUMNS);

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLUMNS; col++)
            {
                // Put a new enemy directly inside the vector, important to avoid reference issues with the textures
                if (row == 0) enemies.emplace_back(EnemyType::SQUID, row, col);
                else if (row == 1 || row == 2) enemies.emplace_back(EnemyType::CRAB, row, col);
                else if (row == 3 || row == 4) enemies.emplace_back(EnemyType::OCTOPUS, row, col);

                // Set the center of the newly added enemy using .back(), 
                // returns a reference to the last enemy in the vector
                enemies.back().setCenter(Vector2f(startX + col * SPACING_X, startY + row * SPACING_Y));
                enemies.back().setStatic(true);

                world.AddPhysicsBody(enemies.back());
            }
        }
    }

public:
    /**
     * @brief Gets the y position of the furthest advanced enemy.
     *
     * @return The y position.
     */
    float getFurthestAdvancedEnemyPosition()
    {
        return enemyController.getFurthestAdvancedEnemyPosition();
    }

    /**
     * @brief Returns the number of active enemies.
     * 
     * @return The active enemy count.
     */
    int getActiveEnemyCount() const
    {
        int count = 0;

        for (const Enemy& enemy : enemies)
        {
            if (enemy.isActive()) count++;
        }

        return count;
    }

    /**
     * @brief Removes an enemy from the game.
     *
     * @param enemy The enemy to remove.
     */
    void removeEnemy(Enemy& enemy)
    {
        if (enemy.isActive())
        {
            enemy.die();
            enemyController.decrementMovementInterval(15);
            world.RemovePhysicsBody(enemy);
        }
    }

    /**
     * @brief Removes the ufo from the game.
     */
    void removeUFO()
    {
        ufoController.removeUFO();
    }

    /**
     * @brief Returns a non const reference to the player.
     *
     * @return The non const reference.
     */
    Player& getPlayer()
    {
        return player;
    }

    /**
     * @brief Returns a const reference to the player.
     *
     * @return The const reference.
     */
    const Player& getPlayer() const
    {
        return player;
    }

    /**
     * @brief Returns a non const vector containing all the enemies.
     *
     * @return The non const vector of enemies.
     */
    std::vector<Enemy>& getEnemies()
    {
        return enemies;
    }

    /**
     * @brief Returns a const vector containing all the enemies.
     *
     * @return The const vector of enemies.
     */
    const std::vector<Enemy>& getEnemies() const
    {
        return enemies;
    }

    /**
     * @brief Returns a non const reference to the ufo.
     *
     * @return The non const reference.
     */
    UFO& getUFO()
    {
        return ufo;
    }

    /**
     * @brief Returns a const reference to the ufo.
     *
     * @return The const reference.
     */
    const UFO& getUFO() const
    {
        return ufo;
    }

    /**
     * @brief Updates all the enemies and the player.
     * 
     * @param deltaMS The elapsed time in milliseconds.
     */
    void update(long deltaMS)
    {
        if (checkGameStateCallback() == State::PAUSED)
        {
            if (!playerController.updateHit(player, deltaMS)) setGameStatePlayingCallback();
            return;
        }

        playerController.update(player, deltaMS);
        enemyController.update(enemies, deltaMS);
        ufoController.update(deltaMS);
    }
};