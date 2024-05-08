#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

namespace games {

    namespace detail {

        float calculateRotationOffset(size_t characterCount) {
            if (characterCount != 4)
                return 90.0f + (360.0f / characterCount) / 2.0f;
            return 0.0f;
        }

        sf::Vector2f calculatePosition(float const angle, float const distance, sf::Vector2f const& center) {
            float const radians = angle * (3.14159f / 180.0f);
            float const offsetX = distance * std::cos(radians);
            float const offsetY = distance * std::sin(radians);
            return { center.x + offsetX, center.y + offsetY };
        }

        void setupLetter(sf::Text& letter, sf::Font const& font, wchar_t const character, sf::Vector2f const& position) {
            letter.setFont(font);
            letter.setCharacterSize(42);
            letter.setString(sf::String(character));

            sf::FloatRect const textBounds = letter.getLocalBounds();
            letter.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
            letter.setPosition(position);
        }

    }

    void loadWheelBackground(sf::CircleShape& shape, float radius) {
        shape.setRadius(radius);
        shape.setPointCount(45);
        shape.setOrigin(radius, radius);
        shape.setFillColor(sf::Color(255, 0, 0, 75));
        shape.setPosition(sf::Vector2f(800 / 2, 600 - radius - 5));
    }

    void loadPlayWheel(std::wstring const& characters, std::vector<sf::Text>& letters_to_play, sf::Vector2f const& center, float const distance, sf::Font const& font) {
        float const rotationOffset = detail::calculateRotationOffset(characters.length());

        for (size_t i = 0; i < characters.length(); ++i) {
            float const angle = i * (360.0f / characters.length()) + rotationOffset;
            sf::Vector2f const position = detail::calculatePosition(angle, distance, center);

            letters_to_play.emplace_back();
            detail::setupLetter(letters_to_play.back(), font, characters[i], position);
        }
    }

    void loadGridCells(auto& word_list, std::vector<sf::Text>& letters, std::vector<sf::Sprite>& sprites, sf::Font& font, sf::Texture& grid_texture) {
        size_t word_index = 0;
        std::ranges::for_each(word_list, [&](auto const& word_def_pair) {
            size_t letter_index = 0;
            std::ranges::for_each(word_def_pair.first, [&](auto const& c) {
                letters.emplace_back();

                letters.back().setFont(font);
                letters.back().setCharacterSize(42);
                letters.back().setString(c);
                letters.back().setFillColor(sf::Color(0, 0, 0, 255));

                sf::FloatRect const textBounds = letters.back().getLocalBounds();
                letters.back().setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                letters.back().setPosition(52 * letter_index + 48, 52 * word_index + 48);

                sprites.emplace_back();
                sprites.back().setTexture(grid_texture);
                sprites.back().setOrigin(16, 16);
                sprites.back().setScale(1.5, 1.5);
                sprites.back().setPosition(52 * letter_index + 48, 52 * word_index + 48);
                sprites.back().setColor(sf::Color(255, 255, 255, 122));
                letter_index++;
                });
            word_index++;
            });
    }

}