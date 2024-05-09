#pragma once
#include <array>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <game_operations.hpp>

namespace games {

	namespace detail {

		void handleMousePressed(sf::Event& window_event, std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			sf::Vector2f mousepos = sf::Vector2f(window_event.mouseButton.x, window_event.mouseButton.y);
			std::ranges::for_each(level_resources->playable_letters, [&mousepos, &level_resources](auto& text) {
				if (text.first.getGlobalBounds().contains(mousepos))
					level_resources->active_word = text.first.getString();
				});
		}

		void handleMouseReleased(sf::Event& window_event, std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			sf::Vector2f mousepos = sf::Vector2f(window_event.mouseButton.x, window_event.mouseButton.y);
			// Check active word against words

			level_resources->active_word = L"";
		}

		void handleMouseMoved(sf::Event& window_event, std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2f mousepos = sf::Vector2f(sf::Mouse::getPosition(shared_resources->game_window));
				std::ranges::for_each(level_resources->playable_letters, [&mousepos, &level_resources](auto& text) {
					if (text.first.getGlobalBounds().contains(mousepos) && std::ranges::find(level_resources->active_word, text.first.getString()) == level_resources->active_word.end())
						level_resources->active_word += text.first.getString();
					});
			}
		}

		void processWindowEvent(sf::Event& window_event, std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			if (window_event.type == sf::Event::Closed)
				shared_resources->game_window.close();
			else if (window_event.type == sf::Event::Resized) {
				shared_resources->game_window.setView(sf::View({ 0, 0, static_cast<float>(window_event.size.width), static_cast<float>(window_event.size.height) }));
				setWheelPosition(shared_resources->wheel_background, level_resources->playable_letters, { window_event.size.width / 2.f, window_event.size.height - 125.f });
			}
			else if (window_event.type == sf::Event::MouseButtonPressed)
				handleMousePressed(window_event, shared_resources, level_resources);
			else if (window_event.type == sf::Event::MouseButtonReleased)
				handleMouseReleased(window_event, shared_resources, level_resources);
			else if (window_event.type == sf::Event::MouseMoved)
				handleMouseMoved(window_event, shared_resources, level_resources);
		}

	}

	void runGameLoop(std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
		while (shared_resources->game_window.isOpen()) {
			sf::Event window_event;
			while (shared_resources->game_window.pollEvent(window_event))
				detail::processWindowEvent(window_event, shared_resources, level_resources);

			shared_resources->game_window.clear();
			shared_resources->game_window.draw(level_resources->background);

			if (level_resources->level_type == LevelResources::LevelType::CROSSWORD) {

				shared_resources->game_window.draw(shared_resources->wheel_background);

				std::ranges::for_each(level_resources->grid_cells, [&shared_resources](auto const& sprite) {
					shared_resources->game_window.draw(sprite);
					});

				std::ranges::for_each(level_resources->playable_letters, [&shared_resources, &level_resources](auto& c) {
					if (std::ranges::find(level_resources->active_word, c.first.getString()) != level_resources->active_word.end())
						shared_resources->game_window.draw(c.second);
					shared_resources->game_window.draw(c.first);
					});

				std::ranges::for_each(level_resources->grid_letters, [&shared_resources](auto const& text) {
					shared_resources->game_window.draw(text);
					});

			}

			shared_resources->game_window.display();
		}
	}

}