#pragma once
#include <chrono>
#include <random>

#include <file_operations.hpp>
#include <graphics_operations.hpp>
#include <string_operations.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace games {

	namespace detail {

		constexpr size_t VOCAB_SIZE = 219;

	}

	struct SharedResources {
		size_t level = 1;

		sf::Font alice;
		sf::Texture cell_background;
		std::array<std::pair<std::wstring, std::wstring>, detail::VOCAB_SIZE> word_def_map;

		sf::CircleShape wheel_background;
	};

	struct LevelResources {
		enum class LevelType { MENU, CROSSWORD } level_type = LevelType::MENU;

		sf::Texture background_texture;
		sf::Sprite background;

		std::vector<sf::Text> playable_letters;
		std::vector<sf::Text> grid_letters;
		std::vector<sf::Sprite> grid_cells;
	};

	bool loadSharedResources(std::unique_ptr<SharedResources>& resources) {
		bool loaded = true;
		loaded &= resources->alice.loadFromFile("../resources/Alice.ttf");
		loaded &= loadVocabularyFromFile("../resources/vocab.txt", resources->word_def_map);
		loaded &= resources->cell_background.loadFromFile("../resources/grid_cell.png");

		// SORT WORD MAP BY LENGTH OF WORD
		std::ranges::sort(resources->word_def_map, [](auto& lft, auto& rht)->bool {
			return lft.first.length() >= rht.first.length();
			});

		loadWheelBackground(resources->wheel_background, 100.f);

		return loaded;
	}

	bool loadLevelResources(std::unique_ptr<SharedResources> const& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
		if (shared_resources->level <= 0)
			level_resources->level_type = LevelResources::LevelType::MENU;
		else
			level_resources->level_type = LevelResources::LevelType::CROSSWORD;

		loadPlayWheel(shared_resources->word_def_map[0].first, level_resources->playable_letters, shared_resources->wheel_background.getPosition(), 75.f, shared_resources->alice);

		auto letters = shared_resources->word_def_map[0].first;
		auto filtered_words = std::ranges::filter_view(shared_resources->word_def_map, [&letters](auto const& word_def_pair)->bool {
			return canSpell(letters, word_def_pair.first);
			});

		loadGridCells(filtered_words, level_resources->grid_letters, level_resources->grid_cells, shared_resources->alice, shared_resources->cell_background);

		return true;
	}

	void cleanUpLevelResources(std::unique_ptr<LevelResources>& resources, size_t level) {

	}

}