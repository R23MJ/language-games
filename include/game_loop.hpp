#pragma once
#include <array>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <game_operations.hpp>

namespace games {

	namespace detail {

		void handlePressed(sf::Vector2f const& pos, std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			std::ranges::for_each(level_resources->playable_letters, [&pos, &level_resources](auto& text) {
				if (text.second.getGlobalBounds().contains(pos))
					level_resources->active_word.push_back(text.first);
				});
		}

		void handleReleased(std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			auto filtered_words = std::ranges::filter_view(shared_resources->word_def_map, [&level_resources](auto const& word_def_pair)->bool {
				return canSpell(level_resources->level_word, word_def_pair.first);
				});

			size_t count = std::ranges::distance(filtered_words);
			auto index = 0;
			std::wstring_view word_ref;
			for (auto& word : filtered_words) {
				size_t letter_index = 0;
				if (word.first.size() == level_resources->active_word.size() && std::ranges::all_of(level_resources->active_word, [&word, &letter_index, &level_resources](auto const& c)->bool {
					return word.first[letter_index++] == c.get().getString();
					})) {
					word_ref = word.first;
					break;
				}
				index += word.first.length();
			}

			for (auto jndex = 0; jndex < word_ref.size(); ++jndex) {
				auto& grid_letter = level_resources->grid_letters[index + jndex];
				if (grid_letter.getFillColor() != sf::Color::Green) {
					level_resources->color_animations.animate(
						sf::Color::Transparent, sf::Color::Green, 0.5f, [&grid_letter](auto const& color) {
							grid_letter.setFillColor(color);
						}, [](const sf::Color& start, const sf::Color& end, float t) {
							sf::Color lerped;
							lerped.r = start.r + (end.r - start.r) * t;
							lerped.g = start.g + (end.g - start.g) * t;
							lerped.b = start.b + (end.b - start.b) * t;
							lerped.a = start.a + (end.a - start.a) * t;
							return lerped;
						});

					level_resources->vec2f_animations.animate(
						shared_resources->wheel_background.getPosition(),
						grid_letter.getPosition(), 0.5f, [&grid_letter](auto const& vec2f) {
							grid_letter.setPosition(vec2f);
						});
				}
			}

			level_resources->completed = std::ranges::all_of(level_resources->grid_letters, [](auto const& c)->bool {
				return c.getFillColor() != sf::Color::Transparent;
				});

			level_resources->active_word.clear();
		} 

		void handleMoved(sf::Vector2f const& pos, std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Touch::isDown(0)) {
				std::ranges::for_each(level_resources->playable_letters, [&pos, &level_resources](auto& text) {
					if (text.second.getGlobalBounds().contains(pos)) {
						if (std::ranges::none_of(level_resources->active_word, [&text](auto const& c)->bool { return &c.get() == &text.first; }))
							level_resources->active_word.push_back(text.first);
						else if (level_resources->active_word.size() >= 2 && &level_resources->active_word[level_resources->active_word.size() - 2].get() == &text.first)
							level_resources->active_word.pop_back();
					}
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
			else if (window_event.type == sf::Event::MouseButtonReleased || window_event.type == sf::Event::TouchEnded) {
				handleReleased(shared_resources, level_resources);
			}
			else if (window_event.type == sf::Event::MouseButtonPressed) {
				sf::Vector2f mousepos = sf::Vector2f(window_event.mouseButton.x, window_event.mouseButton.y);
				handlePressed(mousepos, shared_resources, level_resources);
			}
			else if (window_event.type == sf::Event::MouseMoved) {
				sf::Vector2f mousepos = sf::Vector2f(sf::Mouse::getPosition(shared_resources->game_window));
				handleMoved(mousepos, shared_resources, level_resources);
			}
			else if (window_event.type == sf::Event::TouchBegan) {
				sf::Vector2f mousepos = sf::Vector2f(window_event.touch.x, window_event.touch.y);
				handlePressed(mousepos, shared_resources, level_resources);
			}
			else if (window_event.type == sf::Event::TouchMoved) {
				sf::Vector2f mousepos = sf::Vector2f(sf::Touch::getPosition(0, shared_resources->game_window));
				handleMoved(mousepos, shared_resources, level_resources);
			}
		}

	}

	void runGameLoop(std::unique_ptr<SharedResources>& shared_resources, std::unique_ptr<LevelResources>& level_resources) {
		sf::Clock animation_stop_watch;
		while (shared_resources->game_window.isOpen()) {
			sf::Event window_event;
			while (shared_resources->game_window.pollEvent(window_event))
				detail::processWindowEvent(window_event, shared_resources, level_resources);

			level_resources->color_animations.update(animation_stop_watch.getElapsedTime().asSeconds());
			level_resources->vec2f_animations.update(animation_stop_watch.restart().asSeconds());

			if (level_resources->completed) return;

			shared_resources->game_window.clear();
			shared_resources->game_window.draw(level_resources->background);

			if (level_resources->level_type == LevelResources::LevelType::CROSSWORD) {

				shared_resources->game_window.draw(shared_resources->wheel_background);

				std::ranges::for_each(level_resources->grid_cells, [&shared_resources](auto const& sprite) {
					shared_resources->game_window.draw(sprite);
					});

				std::ranges::for_each(level_resources->playable_letters, [&shared_resources, &level_resources](auto& c) {
					if (std::ranges::find_if(level_resources->active_word, [&c](auto& ref)->bool {
						return &ref.get() == &c.first;
						}) != level_resources->active_word.end())
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