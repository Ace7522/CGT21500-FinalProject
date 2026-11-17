#pragma once

#include <cstdint>

class KeyboardInputTypes
{
public:
    static constexpr std::uint8_t W_KEY = 1;
    static constexpr std::uint8_t S_KEY = 1 << 1;
    static constexpr std::uint8_t A_KEY = 1 << 2;
    static constexpr std::uint8_t D_KEY = 1 << 3;

    static constexpr std::uint8_t Q_KEY = 1 << 4;
    static constexpr std::uint8_t E_KEY = 1 << 5;
};

class PowerUpTypes
{
public:
    static constexpr std::uint8_t SPEED = 1;
    static constexpr std::uint8_t MULTISHOT = 1 << 1;
    static constexpr std::uint8_t RAPIDFIRE = 1 << 2;
};