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

    void createPlayWheel(std::wstring const& characters, std::vector<sf::Text>& letters_to_play, sf::Vector2f const& center, float const distance, sf::Font const& font) {
        float const rotationOffset = detail::calculateRotationOffset(characters.length());

        for (size_t i = 0; i < characters.length(); ++i) {
            float const angle = i * (360.0f / characters.length()) + rotationOffset;
            sf::Vector2f const position = detail::calculatePosition(angle, distance, center);

            letters_to_play.emplace_back();
            detail::setupLetter(letters_to_play.back(), font, characters[i], position);
        }
    }

}