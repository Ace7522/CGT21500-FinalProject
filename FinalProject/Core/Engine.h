#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "GameStateSystem.h"
#include "Renderer.h"
#include "ProjectileSystem.h"
#include "SoundSystem.h"
#include "PowerUpSystem.h"
#include "EntityManager.h"
#include "GameUI.h"
#include "../Game/GenerateBounds.h"

using namespace sf;
using namespace sfp;

class Engine
{
private:
    World& world;
    std::vector<Wall> walls;
    GameStateSystem gameStateSystem;
    Renderer renderer;
    ProjectileSystem projectileSystem;
    SoundSystem soundSystem;
    PowerUpSystem powerUpSystem;
    EntityManager entityManager;
    GameUI gameUI;

public:
    /**
     * @brief Initializes the game engine and its core systems.
     *
     * @param window Reference to the SFML render window.
     * @param world Reference to the physics world.
     */
    Engine(RenderWindow& window, World& world)
        : world(world), walls(generateBounds(window, 20, 200)), gameStateSystem(window.getSize()), 
            renderer(window, world), projectileSystem(world), soundSystem(), powerUpSystem(world, window.getSize()), 
            entityManager(
                world, window.getSize(), walls, projectileSystem, soundSystem, powerUpSystem,
                [this]() { return checkGameState(); }, [this]() { gameStateSystem.setState(State::PLAYING); }
            ),
            gameUI(window.getSize())
    {
        // Add physics bodies for the entities
        world.AddPhysicsBody(entityManager.getPlayer());

        for (Wall& wall : walls)
        {
            world.AddPhysicsBody(wall.rectangle);
        }

        // Initialize the various collisions
        initializeCollisions();
    }

private:
    /**
     * @brief Checks whether the game has been won or lost.
     */
    State checkGameState()
    {
        if (
            gameUI.getCurrentLives() <= 0 || 
            entityManager.getFurthestAdvancedEnemyPosition() > 1550.0f
        ) {
            gameStateSystem.setState(State::LOSE);
        }
        else if (entityManager.getActiveEnemyCount() == 0)
        {
            gameStateSystem.setState(State::WIN);
        }

        return gameStateSystem.getState();
    }

    /**
     * @brief Handles input related to game state transitions.
     */
    void handleGameStateInput()
    {
        if (gameStateSystem.getState() == State::MAINMENU)
        {
            if (KeyboardInput::qKey()) gameStateSystem.setState(State::PLAYING);
        }
    }

    /**
     * @brief Initializes the player collisions.
     */
    void playerCollisions()
    {
        Player& player = entityManager.getPlayer();

        player.onCollision = [this, &player](PhysicsBodyCollisionResult result)
            {
                for (Projectile& projectile : projectileSystem.getProjectiles())
                {
                    if (result.object2 == projectile && projectile.getProjectileType() == ProjectileType::ENEMY)
                    {
                        projectileSystem.removeProjectile(projectile);
                        soundSystem.playSound(SoundTypes::PLAYER_DEATH);
                        gameUI.decrementLivesText();
                        checkGameState();
                        player.hit();
                        gameStateSystem.setState(State::PAUSED);
                        break;
                    }
                }
            };
    }

    /**
     * @brief Initializes the enemy collisions.
     */
    void enemyCollisions()
    {
        for (Enemy& enemy : entityManager.getEnemies())
        {
            enemy.onCollision = [this, &enemy](PhysicsBodyCollisionResult result)
                {
                    for (Projectile& projectile : projectileSystem.getProjectiles())
                    {
                        if (result.object2 == projectile && projectile.getProjectileType() == ProjectileType::PLAYER)
                        {
                            projectileSystem.removeProjectile(projectile);
                            entityManager.removeEnemy(enemy);
                            soundSystem.playSound(SoundTypes::ENEMY_DEATH);
                            gameUI.incrementScoreText(enemy.getScoreValue());
                            checkGameState();
                            break;
                        }
                    }
                };
        }
    }

    /**
     * @brief Initializes the ufo collisions.
     */
    void ufoCollisions()
    {
        UFO& ufo = entityManager.getUFO();

        ufo.onCollision = [this, &ufo](PhysicsBodyCollisionResult result)
            {
                for (Projectile& projectile : projectileSystem.getProjectiles())
                {
                    if (result.object2 == projectile && projectile.getProjectileType() == ProjectileType::PLAYER)
                    {
                        projectileSystem.removeProjectile(projectile);
                        entityManager.removeUFO();
                        gameUI.incrementScoreText(ufo.getScoreValue());
                        soundSystem.stopSound(SoundTypes::UFO_MOVEMENT);
                        break;
                    }
                }
            };
    }

    /**
     * @brief Initializes the projectile collisions.
     */
    void projectileCollisions()
    {
        for (Projectile& projectile : projectileSystem.getProjectiles())
        {
            projectile.onCollision = [this, &projectile](PhysicsBodyCollisionResult result)
                {
                    for (Wall& wall : walls)
                    {
                        if (result.object2 == wall.rectangle)
                        {
                            projectileSystem.removeProjectile(projectile);
                            return;
                        }
                    }

                    for (PowerUp& powerUp : powerUpSystem.getPowerUps())
                    {
                        if (powerUp.isActive() && result.object2 == powerUp)
                        {
                            projectileSystem.removeProjectile(projectile);
                            if (projectile.getProjectileType() == ProjectileType::ENEMY) powerUpSystem.removePowerUp(powerUp);
                            return;
                        }
                    }
                };
        }
    }

    /**
     * @brief Initializes the power up collisions.
     */
    void powerUpCollisions()
    {
        for (PowerUp& powerUp : powerUpSystem.getPowerUps())
        {
            powerUp.onCollision = [this, &powerUp](PhysicsBodyCollisionResult result)
                {
                    if (result.object2 == entityManager.getPlayer())
                    {
                        powerUpSystem.activatePowerUp(powerUp.getType());
                        powerUpSystem.removePowerUp(powerUp);
                    }
                };
        }
    }

    /**
     * @brief Initializes all the collisions.
     */
    void initializeCollisions()
    {
        playerCollisions();
        enemyCollisions();
        ufoCollisions();
        projectileCollisions();
        powerUpCollisions();
    }

public:
    /**
     * @brief Updates the systems.
     *
     * @param deltaMS The elapsed time in milliseconds.
     */
    void update(long deltaMS)
    {
        handleGameStateInput();
        if (
            gameStateSystem.getState() == State::MAINMENU ||
            gameStateSystem.getState() == State::WIN ||
            gameStateSystem.getState() == State::LOSE
        ) return;

        entityManager.update(deltaMS);
        if (gameStateSystem.getState() == State::PAUSED) return;

        world.UpdatePhysics(deltaMS);
        powerUpSystem.update(deltaMS);
        checkGameState();
    }

    /**
     * @brief Renders the screen.
     */
    void render()
    {
        renderer.clearScreen();

        switch (gameStateSystem.getState())
        {
            case State::MAINMENU:
                renderer.render(gameStateSystem);
                break;

            case State::PLAYING:
            case State::PAUSED:
                renderer.render(entityManager, projectileSystem, powerUpSystem, gameUI);
                break;

            case State::WIN:
            case State::LOSE:
                renderer.render(entityManager, projectileSystem, powerUpSystem, gameUI);
                renderer.render(gameStateSystem);
                break;
        }

        renderer.displayScreen();
    }
};