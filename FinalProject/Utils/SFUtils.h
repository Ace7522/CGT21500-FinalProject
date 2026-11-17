#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

/**
 * @brief Loads the texture image into the given texture.
 *
 * @param texture The texture.
 * @param texture The file path to the image.
 */
void LoadTex(Texture& texture, const std::string& filePath)
{
    if (!texture.loadFromFile(filePath))
    {
        std::cout << "Could not load " << filePath << std::endl;
    }
}

/**
 * @brief Gets the width and height of the given text.
 *
 * @param text The text element.
 */
Vector2f GetTextSize(const Text& text)
{
    FloatRect r = text.getGlobalBounds();
    return Vector2f(r.width, r.height);
}

/**
 * @brief Initializes the text.
 *
 * @param text The text element.
 * @param font The font to use for the text.
 * @param content The content of the text.
 * @param position The position to place the text.
 * @param centered Whether to center the text at its given position.
 * @param fillColor The fill color of the text.
 * @param fontSize The font size of the text.
 * @param letterSpacing The amount of space between letters.
 */
void initializeText(
    Text& text, Font& font, const std::string& content, Vector2f position, bool centered,
    Color fillColor = Color::White, int fontSize = 92, float letterSpacing = 2.5f
) {
    text.setFont(font);
    text.setString(content);
    text.setFillColor(fillColor);
    text.setCharacterSize(fontSize);
    text.setLetterSpacing(letterSpacing);
    text.setStyle(Text::Bold);

    if (centered)
    {
        Vector2f textSize = GetTextSize(text);
        text.setPosition(Vector2f(position.x - textSize.x / 2, position.y - textSize.y / 2));
    }
    else
    {
        text.setPosition(position);
    }
}