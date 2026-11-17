#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "../Utils/SFUtils.h"

using namespace sf;
using namespace sfp;

enum class EnemyType
{
    CRAB,
    OCTOPUS,
    SQUID
};

struct EnemyData
{
    std::string filePath1;
    std::string filePath2;
    int scoreValue;
};

class Enemy : public PhysicsSprite
{
private:
    EnemyType type;
    Texture movementTextureOne;
    Texture movementTextureTwo;
    Texture deathTexture;
    bool usingMovementTextureOne = true;
    bool active = true;
    int row;
    int col;
    bool canShoot = false;

    bool isDying = false;
    long deathTimer = 0;
    long deathDuration = 150;

public:
    /**
     * @brief Creates an enemy.
     * 
     * @param type The enemy type
     */
    Enemy(EnemyType type, int row, int col)
        : type(type), row(row), col(col)
    {
        EnemyData data = GetEnemyData(type);
        loadTextures(data.filePath1, data.filePath2);
        LoadTex(deathTexture, "Assets/Images/alienDeath.png");
        setTexture(movementTextureOne);
        setSize(Vector2f(movementTextureOne.getSize().x, movementTextureOne.getSize().y));
    }

private:
    /**
     * @brief Loads the frame textures for the enemy.
     *
     * @param filePath1 The frame one file path.
     * @param filePath2 The frame two file path.
     */
    void loadTextures(const std::string& filePath1, const std::string& filePath2)
    {
        LoadTex(movementTextureOne, filePath1);
        LoadTex(movementTextureTwo, filePath2);
    }

    /**
     * @brief Switches the current texture.
     */
    void updateTexture()
    {
        if (usingMovementTextureOne)
        {
            setTexture(movementTextureTwo);
            usingMovementTextureOne = false;
        }
        else
        {
            setTexture(movementTextureOne);
            usingMovementTextureOne = true;
        }
    }

    /**
     * @brief Gets the data associated with an enemy type.
     * 
     * @param type The enemy type.
     * 
     * @returns The enemy data.
     */
    EnemyData GetEnemyData(EnemyType type) const
    {
        switch (type)
        {
            case EnemyType::OCTOPUS: return { "Assets/Images/octopus1.png", "Assets/Images/octopus2.png", 10 };
            case EnemyType::CRAB: return { "Assets/Images/crab1.png", "Assets/Images/crab2.png", 20 };
            case EnemyType::SQUID: return { "Assets/Images/squid1.png", "Assets/Images/squid2.png", 30 };
            default: return { "Assets/Images/octopus1.png", "Assets/Images/octopus2.png", 10 };
        }
    }

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
     * @brief Sets a enemy's active state.
     *
     * @param state The new state.
     */
    void setActive(bool state)
    {
        active = state;
    }

    /**
     * @brief Gets the score value associated with the enemy.
     *
     * @return The score value.
     */
    int getScoreValue() const
    {
        return GetEnemyData(type).scoreValue;
    }

    /**
     * @brief Gets the enemy's formation row.
     *
     * @return The enemy's row.
     */
    int getRow() const
    {
        return row;
    }

    /**
     * @brief Gets the enemy's formation column.
     *
     * @return The enemy's column.
     */
    int getCol() const
    {
        return col;
    }

    /**
     * @brief Gets the enemy's can shoot state.
     *
     * @return True of the enemy can shoot, otherwise false.
     */
    bool getCanShoot() const
    {
        return canShoot;
    }

    /**
     * @brief Sets the enemy's can shoot state.
     *
     * @param The new state.
     */
    void setCanShoot(bool state)
    {
        canShoot = state;
    }

    /**
     * @brief Starts the enemy death animation.
     */
    void die()
    {
        if (isDying) return;

        isDying = true;
        deathTimer = deathDuration;

        setTexture(deathTexture);
    }

    /**
     * @brief Gets the enemy's dying state.
     *
     * @return True of the enemy is dying, otherwise false.
     */
    bool getIsDying() const
    {
        return isDying;
    }

    /**
     * @brief Updates the enemy death.
     *
     * @param deltaMS The elapsed time in milliseconds.
     */
    void updateDying(long deltaMS)
    {
        if (!isDying) return;

        deathTimer -= deltaMS;
        if (deathTimer <= 0) active = false;
    }

    /**
     * @brief Updates the enemy.
     */
    void update(long deltaMS)
    {
        if (isDying) return;

        updateTexture();
    }
};