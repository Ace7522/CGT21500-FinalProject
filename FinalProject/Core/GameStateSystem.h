#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Utils/SFUtils.h"

using namespace sf;

enum class State
{
    MAINMENU,
    PLAYING,
    PAUSED,
    WIN,
    LOSE
};

class GameStateSystem
{
private:
    Vector2u windowSize;
    Font font;

    State currentState = State::MAINMENU;

    Text startGameHeaderText;
    Text startGameSubTitleText;
    Text startGameBodyText;
    Text winText;
    Text loseText;

public:
    /**
     * @brief Initializes the game state system.
     *
     * @param windowSize Size of the game window.
     */
    GameStateSystem(Vector2u windowSize)
        : windowSize(windowSize)
    {
        if (!font.loadFromFile("Assets/Fonts/Delicatus-e9OLl.ttf"))
        {
            std::cout << "Could not load font." << std::endl;
            exit(3);
        }

        initializeText(startGameHeaderText, font, "Space Invaders", Vector2f(windowSize.x / 2, 510.0f), true, Color::White, 175, 2.0f);
        initializeText(startGameSubTitleText, font, "Press \"Q\" to begin.", Vector2f(windowSize.x / 2, 710.0f), true);
        initializeText(startGameBodyText, font, "WASD to move, E to fire.", Vector2f(windowSize.x / 2, 1010.0f), true, Color(155, 155, 155));
        initializeText(winText, font, "You Win", Vector2f(windowSize.x / 2, 410.0f), true, Color::Green, 175, 1.5f);
        initializeText(loseText, font, "Game Over", Vector2f(windowSize.x / 2, 410.0f), true, Color::Red, 175, 1.5f);
    }

    /**
     * @brief Returns the current game state.
     */
    State getState() const
    {
        return currentState;
    }

    /**
     * @brief Changes the current game state.
     * 
     * @param state The new state.
     */
    void setState(State state)
    {
        currentState = state;
    }

    /**
     * @brief Returns the text elements for the current game state.
     * 
     * @returns The vector of text.
     */
    const std::vector<Text> getCurrentElements() const
    {
        switch (currentState)
        {
            case State::MAINMENU: return { startGameHeaderText, startGameSubTitleText, startGameBodyText };
            case State::WIN: return { winText };
            case State::LOSE: return { loseText };
            default: return { };
        }
    }
};