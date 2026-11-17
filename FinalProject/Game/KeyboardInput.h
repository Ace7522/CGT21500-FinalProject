#pragma once

#include <iostream>
#include <cstdint>
#include "../Core/GameFlags.h"

class KeyboardInput {
public:
    /**
     * @brief Checks if the W key is pressed.
     *
     * @return 0b00000001 if the W key is pressed, else 0b00000000.
     */
    static std::uint8_t wKey()
    {
        return Keyboard::isKeyPressed(Keyboard::W) ? KeyboardInputTypes::W_KEY : 0;
    }

    /**
     * @brief Checks if the S key is pressed.
     *
     * @return 0b00000010 if the S key is pressed, else 0b00000000.
     */
    static std::uint8_t sKey()
    {
        return Keyboard::isKeyPressed(Keyboard::S) ? KeyboardInputTypes::S_KEY : 0;
    }

    /**
     * @brief Checks if the A key is pressed.
     *
     * @return 0b00000100 if the A key is pressed, else 0b00000000.
     */
    static std::uint8_t aKey()
    {
        return Keyboard::isKeyPressed(Keyboard::A) ? KeyboardInputTypes::A_KEY : 0;
    }

    /**
     * @brief Checks if the D key is pressed.
     *
     * @return 0b0001000 if the D key is pressed, else 0b00000000.
     */
    static std::uint8_t dKey()
    {
        return Keyboard::isKeyPressed(Keyboard::D) ? KeyboardInputTypes::D_KEY : 0;
    }

    /**
     * @brief Checks if the E key is pressed.
     *
     * @return 0b0010000 if the E key is pressed, else 0b00000000.
     */
    static std::uint8_t eKey()
    {
        return Keyboard::isKeyPressed(Keyboard::E) ? KeyboardInputTypes::E_KEY : 0;
    }

    /**
     * @brief Checks if the Q key is pressed.
     *
     * @return 0b0100000 if the Q key is pressed, else 0b00000000.
     */
    static std::uint8_t qKey()
    {
        return Keyboard::isKeyPressed(Keyboard::Q) ? KeyboardInputTypes::Q_KEY : 0;
    }

    /**
     * @brief Checks what keys are currently pressed.
     *
     * @return A bit mask containing 1s where the corresponding key is pressed.
     */
    static std::uint8_t update()
    {
        return wKey() | sKey() | aKey() | dKey() | eKey() | qKey();
    }
};