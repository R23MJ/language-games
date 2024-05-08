#pragma once
#include <array>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace games {

	namespace detail {

		constexpr size_t VOCAB_SIZE = 219;

		auto processWindowEvent(sf::Event& window_event) {
			if (window_event.type == sf::Event::Closed)
				return false;
			return true;
		}

	}

	struct SharedResources {
		sf::Font alice;
		std::array<std::pair<std::wstring, std::wstring>, detail::VOCAB_SIZE> word_def_map;

		sf::CircleShape wheel_background;
	};

	struct LevelResources {
		sf::Texture background_texture;
		sf::Sprite background;

		std::vector<sf::Text> playable_letters;
		std::vector<std::vector<sf::Text>> crossword_grid;

	};

	void loadWheelBackground(sf::CircleShape& shape, float radius) {
		shape.setRadius(radius);
		shape.setPointCount(45);
		shape.setOrigin(radius, radius);
		shape.setFillColor(sf::Color(255, 0, 0, 75));
		shape.setPosition(sf::Vector2f(800 / 2, 600 - radius - 5));
	}

	void runGameLoop(sf::RenderWindow& game_window, std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
		while (game_window.isOpen()) {
			sf::Event window_event;
			while (game_window.pollEvent(window_event)) {
				if (!detail::processWindowEvent(window_event))
					game_window.close();
			}

			game_window.clear();
			game_window.draw(level_resources->background);

			game_window.draw(shared_resources->wheel_background);

			//std::ranges::for_each(crossword_words, [&game_window](auto const& word) {
			//	game_window.draw(word);
			//	});

			//std::ranges::for_each(letters_to_play, [&game_window](auto const& c) {
			//	game_window.draw(c);
			//	});

			game_window.display();
		}
	}

}