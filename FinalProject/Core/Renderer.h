#pragma once

#include <SFML/Graphics.hpp>
#include "GameStateSystem.h"
#include "EntityManager.h"
#include "ProjectileSystem.h"
#include "PowerUpSystem.h"
#include "GameUI.h"

using namespace sf;

class Renderer
{
private:
	RenderWindow& window;
    World& world;

public:
    /**
     * @brief Initializes the renderer.
     *
     * @param window Reference to the SFML render window.
     * @param world Reference to the physics world.
     */
	Renderer(RenderWindow& window, World& world)
        : window(window), world(world)
	{
	}

private:
    /**
     * @brief Renders all the game state elements.
     *
     * @param gameStateSystem The game state system.
     */
    void renderGameStateText(const GameStateSystem& gameStateSystem)
    {
        for (const Text& text : gameStateSystem.getCurrentElements())
        {
            window.draw(text);
        }
    }

    /**
     * @brief Renders all the game entities.
     *
     * @param entityManager The manager containing the player and game entities.
     */
    void renderEntities(const EntityManager& entityManager)
    {
        window.draw(entityManager.getPlayer());

        for (const auto& enemy : entityManager.getEnemies())
        {
            if (enemy.isActive()) window.draw(enemy);
        }

        if (entityManager.getUFO().isActive()) window.draw(entityManager.getUFO());
    }

    /**
     * @brief Renders all the game projectiles.
     *
     * @param projectileSystem The system containing the projectiles.
     */
    void renderProjectiles(const ProjectileSystem& projectileSystem)
    {
        for (const auto& projectile : projectileSystem.getProjectiles())
        {
            if (projectile.isActive()) window.draw(projectile);
        }
    }

    /**
     * @brief Renders all the game power ups.
     *
     * @param powerUpSystem The system containing the power ups.
     */
    void renderPowerUps(const PowerUpSystem& powerUpSystem)
    {
        for (const auto& powerUp : powerUpSystem.getPowerUps())
        {
            if (powerUp.isActive()) window.draw(powerUp);
        }
    }

    /**
     * @brief Renders all the game ui elements.
     *
     * @param gameUI The game ui.
     */
    void renderGameUI(const GameUI& gameUI)
    {
        for (const Text& text : gameUI.getText())
        {
            window.draw(text);
        }

        window.draw(gameUI.getBottomBorder());

        for (const Sprite& sprite : gameUI.getSprites())
        {
            window.draw(sprite);
        }
    }

    /**
     * @brief Renders all the collision boundaries.
     *
     * @param window Reference to the SFML render window.
     */
    void renderCollisions(RenderWindow& window)
    {
        world.VisualizeAllBounds(window);
    }

public:
    /**
     * @brief Clears the window screen.
     */
    void clearScreen()
    {
        window.clear();
    }

    /**
     * @brief Renders the window screen.
     */
    void displayScreen()
    {
        window.display();
    }

    /**
     * @brief Renders just the game state elements.
     *
     * @param gameStateSystem The game state system.
     */
    void render(const GameStateSystem& gameStateSystem)
    {
        renderGameStateText(gameStateSystem);
    }

    /**
     * @brief Renders all the main game elements.
     *
     * @param entityManager The manager containing the player and game entities.
     * @param projectileSystem The projectile system.
     * @param powerUpSystem The power up system.
     * @param gameUI The game ui.
     */
    void render(
        const EntityManager& entityManager, const ProjectileSystem& projectileSystem,
        const PowerUpSystem& powerUpSystem, const GameUI& gameUI
    ) {
        renderEntities(entityManager);
        renderProjectiles(projectileSystem);
        renderPowerUps(powerUpSystem);
        renderGameUI(gameUI);
    }
};