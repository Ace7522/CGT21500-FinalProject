#pragma once

#include <iostream>
#include <cstdint>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include "KeyboardInput.h"
#include "../Utils/SFUtils.h"

using namespace sf;
using namespace sfp;

class Player : public PhysicsSprite
{
private:
	Texture texture;
    Texture deathTexture1;
    Texture deathTexture2;
    Texture deathTexture3;
    std::function<void(Player&, std::uint8_t, long)> inputCallback;

    bool isHit = false;
    long hitTimer = 0;
    long hitDuration = 2500;
    bool usingDeathTextureOne = true;
    long deathTextureTimer = 0;
    long deathTextureDuration = (2500 - 1000) / 25;

public:
    /**
    * @brief Creates a player.
    *
    * @param windowSize Size of the game window.
    */
    Player(Vector2u windowSize)
    {
		LoadTex(texture, "Assets/Images/ship.png");
        LoadTex(deathTexture1, "Assets/Images/shipDeath1.png");
        LoadTex(deathTexture2, "Assets/Images/shipDeath2.png");
        LoadTex(deathTexture3, "Assets/Images/shipDeath3.png");
        setTexture(texture);
        setSize(Vector2f(texture.getSize().x, texture.getSize().y));
        setCenter(Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));
        setStatic(true);
    }

    /**
     * @brief Sets the input callback used by the player.
     *
     * @param callback The callback to use for the input.
     */
    void setInputCallback(auto callback)
    {
        inputCallback = callback;
    }

    /**
     * @brief Starts the player's hit animation.
     */
    void hit()
    {
        if (isHit) return;

        isHit = true;
        hitTimer = hitDuration;
        setTexture(deathTexture1);
    }

    /**
     * @brief Gets the player's hit state.
     * 
     * @return True if the player is hit, otherwise false.
     */
    bool getHit() const
    {
        return isHit;
    }

    /**
     * @brief Updates the hit animation.
     * 
     * @param deltaMS The elapsed time in milliseconds.
     */
    void updateHit(long deltaMS)
    {
        if (!isHit) return;

        hitTimer -= deltaMS;

        if (hitTimer <= 0)
        {
            isHit = false;
            setTexture(texture);
            return;
        }

        if (hitTimer <= 1000)
        {
            setTexture(deathTexture3);
            return;
        }

        deathTextureTimer += deltaMS;

        if (deathTextureTimer >= deathTextureDuration)
        {
            usingDeathTextureOne = !usingDeathTextureOne;
            setTexture(usingDeathTextureOne ? deathTexture1 : deathTexture2);
            deathTextureTimer = 0;
        }
    }

    /**
     * @brief Updates the player by sending the inputs to the input controller.
     * 
     * @param deltaMS The elapsed time in milliseconds.
     */
    void update(long deltaMS)
    {
        if (isHit) return;

        std::uint8_t input = KeyboardInput::update();
        inputCallback(*this, input, deltaMS);
    }
};