#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Utils/SFUtils.h"

using namespace sf;

class GameUI
{
private:
    Vector2u windowSize;
    Font font;
    int currentScore = 0;
    int currentLives = 3;

    Text headerText;
    Text scoreCounter1;
    Text scoreCounter2;
    Text livesCounter;
    Text creditCounter;

    RectangleShape bottomBorder;

    Texture texture;
    Sprite sprite1;
    Sprite sprite2;

public:
    /**
     * @brief Creates the game ui
     *
     * @param windowSize Size of the game window.
     */
    GameUI(Vector2u windowSize)
        : windowSize(windowSize)
    {
        if (!font.loadFromFile("Assets/Fonts/Delicatus-e9OLl.ttf"))
        {
            std::cout << "Could not load font." << std::endl;
            exit(3);
        }

        initializeText(headerText, font, "SCORE<1> HI-SCORE SCORE<2>", Vector2f(windowSize.x / 2, 30.0f), true);
        initializeText(scoreCounter1, font, "0000", Vector2f(155.0f, 120.0f), false);
        initializeText(scoreCounter2, font, "1980", Vector2f(600.0f, 120.0f), false);
        initializeText(livesCounter, font, "3", Vector2f(50.0f, windowSize.y - 170.0f), false);
        initializeText(creditCounter, font, "CREDIT 03", Vector2f(windowSize.x - 610.0f, windowSize.y - 170.0f), false);

        bottomBorder.setSize(Vector2f(windowSize.x, 5.0f));
        bottomBorder.setPosition(Vector2f(0.f, windowSize.y - 140.0f));
        bottomBorder.setFillColor(Color::Green);

        LoadTex(texture, "Assets/Images/ship.png");
        sprite1.setTexture(texture);
        sprite2.setTexture(texture);
        sprite1.setPosition(Vector2f(130.0f, windowSize.y - 165.0f));
        sprite2.setPosition(Vector2f(215.0f, windowSize.y - 165.0f));
        sprite1.setScale(Vector2f(0.9f, 0.9f));
        sprite2.setScale(Vector2f(0.9f, 0.9f));
    }

    /**
     * @brief Gets the player's current lives count.
     */
    int getCurrentLives() const
    {
        return currentLives;
    }

    /**
     * @brief Increments the score text.
     *
     * @param The amount to increment the score text by.
     */
    void incrementScoreText(int amount)
    {
        currentScore += amount;

        std::string score = std::to_string(currentScore);
        while (score.length() < 4)
        {
            score = "0" + score;
        }

        scoreCounter1.setString(score);
    }

    /**
     * @brief Decrements the lives text by one.
     */
    void decrementLivesText()
    {
        currentLives--;
        std::string lives = std::to_string(currentLives);
        livesCounter.setString(lives);
    }

    /**
     * @brief Returns a vector of the text elements.
     *
     * @return The vector of text.
     */
    const std::vector<Text> getText() const
    {
        return { headerText, livesCounter, creditCounter, scoreCounter1, scoreCounter2 };
    }

    /**
     * @brief Returns the bottom border of the game ui.
     *
     * @return The bottom border.
     */
    const RectangleShape& getBottomBorder() const
    {
        return bottomBorder;
    }

    /**
     * @brief Returns a vector of the sprite elements.
     *
     * @return The vector of sprites.
     */
    const std::vector<Sprite> getSprites() const
    {
        if (currentLives == 3) return { sprite1, sprite2 };
        else if (currentLives == 2) return { sprite1 };
        else return { };
    }
};