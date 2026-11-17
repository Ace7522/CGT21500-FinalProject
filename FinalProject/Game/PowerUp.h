#pragma once

#include <iostream>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "../Core/GameFlags.h"
#include "../Utils/SFUtils.h"

using namespace sf;
using namespace sfp;

class PowerUp : public PhysicsSprite
{
private:
    Texture speedPowerUpTexture;
    Texture multiShotPowerUpTexture;
    Texture fireRatePowerUpTexture;;

    bool active = false;
    std::uint8_t type = 0;

public:
    /**
     * @brief Creates a power up.
     */
    PowerUp()
    {
        LoadTex(speedPowerUpTexture, "Assets/Images/speedPowerUp.png");
        LoadTex(multiShotPowerUpTexture, "Assets/Images/multiShotPowerUp.png");
        LoadTex(fireRatePowerUpTexture, "Assets/Images/fireRatePowerUp.png");
        setStatic(true);
    }

    /**
     * @brief Gets the type of the power up.
     *
     * @return The type of the power up as an unsigned integer.
     */
    std::uint8_t getType() const
    {
        return type;
    }

    /**
     * @brief Sets the type of the power up.
     *
     * @paran type The new type.
     */
    void setType(std::uint8_t type)
    {
        this->type = type;

        switch (type)
        {
            case PowerUpTypes::SPEED: setTexture(speedPowerUpTexture); break;
            case PowerUpTypes::MULTISHOT: setTexture(multiShotPowerUpTexture); break;
            case PowerUpTypes::RAPIDFIRE: setTexture(fireRatePowerUpTexture); break;
        }
    }

    /**
     * @brief Checks if the power up is active.
     *
     * @return True if active, otherwise false.
     */
    bool isActive() const
    {
        return active;
    }

    /**
     * @brief Sets a power up's active state.
     *
     * @param state The new state.
     */
    void setActive(bool state)
    {
        active = state;
    }
};