#pragma once
#include <chrono>
#include <random>

#include <Animator.hpp>
#include <file_operations.hpp>
#include <graphics_operations.hpp>
#include <string_operations.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace games {

	namespace detail {

		constexpr size_t VOCAB_SIZE = 19;
		constexpr size_t TEXTURE_COUNT = 4;

	}

	struct SharedResources {
		size_t level = 1;

		std::mt19937 generator;
		std::uniform_int_distribution<int> distribution;

		sf::RenderWindow game_window;

		sf::Font alice;
		sf::Texture cell_background;
		std::array<sf::Texture, detail::TEXTURE_COUNT> background_textures;
		std::array<std::pair<std::wstring, std::wstring>, detail::VOCAB_SIZE> word_def_map;

		sf::CircleShape wheel_background;
	};

	struct LevelResources {
		enum class LevelType { MENU, CROSSWORD } level_type = LevelType::MENU;

		bool completed = false;

		std::wstring level_word;
		std::vector<std::reference_wrapper<sf::Text>> active_word;

		sf::Sprite background;

		Animation<sf::Vector2f> vec2f_animations;
		Animation<sf::Color> color_animations;

		std::vector<std::pair<sf::Text, sf::CircleShape>> playable_letters;
		std::vector<sf::Text> grid_letters;
		std::vector<sf::Sprite> grid_cells;
	};

	namespace detail {
	
		void getLevelWord(std::unique_ptr<SharedResources> const& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			shared_resources->generator.seed(shared_resources->level);
			level_resources->level_word = shared_resources->word_def_map[shared_resources->distribution(shared_resources->generator)].first;
		}
	
		int rotationalClamp(int value, int minValue, int maxValue) {
			int range = maxValue - minValue + 1;
			while (value < minValue)
				value += range;
			return minValue + (value - minValue) % range;
		}

	}

	bool loadSharedResources(std::unique_ptr<SharedResources>& resources) {
		bool loaded = true;
		loaded &= resources->alice.loadFromFile("../resources/Alice.ttf");
		loaded &= loadVocabularyFromFile("../resources/vocab.txt", resources->word_def_map);
		loaded &= resources->cell_background.loadFromFile("../resources/grid_cell.png");
		loaded &= resources->background_textures[0].loadFromFile("../resources/background.png");
		loaded &= resources->background_textures[1].loadFromFile("../resources/background-2.png");
		loaded &= resources->background_textures[2].loadFromFile("../resources/background-3.png");
		loaded &= resources->background_textures[3].loadFromFile("../resources/background-4.png");

		// SORT WORD MAP BY LENGTH OF WORD
		std::ranges::sort(resources->word_def_map, [](auto& lft, auto& rht)->bool {
			return lft.first.length() >= rht.first.length();
			});

		resources->distribution = std::uniform_int_distribution<int>(0, resources->word_def_map.size() - 1);
		resources->game_window.create(sf::VideoMode(800, 600), "Language Games");

		return loaded;
	}

	bool loadLevelResources(std::unique_ptr<SharedResources> const& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
		if (shared_resources->level <= 0)
			level_resources->level_type = LevelResources::LevelType::MENU;
		else
			level_resources->level_type = LevelResources::LevelType::CROSSWORD;
		
		level_resources->background.setTexture(shared_resources->background_textures[detail::rotationalClamp(shared_resources->level, 0, detail::TEXTURE_COUNT - 1)]);

		detail::getLevelWord(shared_resources, level_resources);
		std::ranges::shuffle(level_resources->level_word, shared_resources->generator);
		std::ranges::for_each(level_resources->level_word, [&level_resources](auto const& c) {
			level_resources->playable_letters.emplace_back().first.setString(c);
			});

		loadPlayWheel(level_resources->playable_letters, shared_resources->wheel_background, shared_resources->alice, { shared_resources->game_window.getSize().x / 2.f, shared_resources->game_window.getSize().y - 125.f });

		auto filtered_words = std::ranges::filter_view(shared_resources->word_def_map, [&level_resources](auto const& word_def_pair)->bool { 
			return canSpell(level_resources->level_word, word_def_pair.first);
			});

		auto take_view = std::ranges::take_view(filtered_words, 10);

		loadGridCells(take_view, level_resources->grid_letters, level_resources->grid_cells, shared_resources->alice, shared_resources->cell_background);

		return true;
	}

	void cleanUpLevelResources(std::unique_ptr<LevelResources>& resources, size_t level) {
		resources->completed = false;
		resources->grid_letters.clear();
		resources->grid_cells.clear();
		resources->playable_letters.clear();
		resources->active_word.clear();
		resources->level_word.clear();
		resources->color_animations.clear();
		resources->vec2f_animations.clear();
	}

}