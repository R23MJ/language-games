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

        void loadWheelBackground(sf::CircleShape& shape, float radius) {
            shape.setRadius(radius);
            shape.setPointCount(45);
            shape.setOrigin(radius, radius);
            shape.setFillColor(sf::Color(255, 0, 0, 105));
        }

        void loadLetter(std::pair<sf::Text, sf::CircleShape>& letter, sf::Font const& font) {
            letter.first.setFont(font);
            letter.first.setCharacterSize(42);

            loadWheelBackground(letter.second, 25);
            letter.first.setFillColor(sf::Color::Black);
            letter.second.setFillColor(sf::Color::White);

            sf::FloatRect const textBounds = letter.first.getLocalBounds();
            letter.first.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
            letter.second.setOrigin(25, 25);
        }

    }

    void setWheelPosition(sf::CircleShape& wheel_bg, std::vector<std::pair<sf::Text, sf::CircleShape>>& playable_letters, sf::Vector2f const& position) {
        float const rotationOffset = detail::calculateRotationOffset(playable_letters.size());

        wheel_bg.setPosition(position);
        for (size_t i = 0; i < playable_letters.size(); ++i) {
            float const angle = i * (360.0f / playable_letters.size()) + rotationOffset;
            sf::Vector2f const letter_position = detail::calculatePosition(angle, wheel_bg.getRadius() - 25.f, position);
            playable_letters[i].first.setPosition(letter_position);
            playable_letters[i].second.setPosition(letter_position);
        }
    }

    void loadPlayWheel(std::vector<std::pair<sf::Text, sf::CircleShape>>& playable_letters, sf::CircleShape& shape, sf::Font const& font, sf::Vector2f const& position) {
        detail::loadWheelBackground(shape, 100.f);
        std::ranges::for_each(playable_letters, [&font](auto& text) {
            detail::loadLetter(text, font);
            });
        setWheelPosition(shape, playable_letters, position);
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
                letters.back().setFillColor(sf::Color::Transparent);

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