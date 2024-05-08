#pragma once
#include <chrono>
#include <random>

#include <file_operations.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace games {

	namespace detail {

		constexpr size_t VOCAB_SIZE = 219;

		void loadWheelBackground(sf::CircleShape& shape, float radius) {
			shape.setRadius(radius);
			shape.setPointCount(45);
			shape.setOrigin(radius, radius);
			shape.setFillColor(sf::Color(255, 0, 0, 75));
			shape.setPosition(sf::Vector2f(800 / 2, 600 - radius - 5));
		}
	}

	struct SharedResources {
		size_t level = 1;

		sf::Font alice;
		std::array<std::pair<std::wstring, std::wstring>, detail::VOCAB_SIZE> word_def_map;

		sf::CircleShape wheel_background;
	};

	struct LevelResources {
		enum class LevelType { MENU, CROSSWORD } level_type = LevelType::MENU;

		sf::Texture background_texture;
		sf::Sprite background;

		std::vector<sf::Text> playable_letters;
		std::vector<std::vector<sf::Text>> crossword_grid;
	};

	bool loadSharedResources(std::unique_ptr<SharedResources>& resources) {
		bool loaded = true;
		loaded &= resources->alice.loadFromFile("../resources/Alice.ttf");
		loaded &= loadVocabularyFromFile("../resources/vocab.txt", resources->word_def_map);

		// SORT WORD MAP BY LENGTH OF WORD
		std::ranges::sort(resources->word_def_map, [](auto& lft, auto& rht)->bool {
			return lft.first.length() >= rht.first.length();
			});

		detail::loadWheelBackground(resources->wheel_background, 100.f);

		return loaded;
	}

	bool loadLevelResources(std::unique_ptr<SharedResources> const& shared, std::unique_ptr<LevelResources>& resources) {
		if (shared->level <= 0)
			resources->level_type = LevelResources::LevelType::MENU;
		else
			resources->level_type = LevelResources::LevelType::CROSSWORD;



		return true;
	}

	void cleanUpLevelResources(std::unique_ptr<LevelResources>& resources, size_t level) {

	}

}