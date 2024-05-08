#pragma once
#include <array>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <game_operations.hpp>

namespace games {

	namespace detail {

		auto processWindowEvent(sf::Event& window_event) {
			if (window_event.type == sf::Event::Closed)
				return false;
			return true;
		}

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

			if (level_resources->level_type == LevelResources::LevelType::CROSSWORD)
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