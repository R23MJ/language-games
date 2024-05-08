#include <chrono>
#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <board_operations.hpp>
#include <file_operations.hpp>
#include <string_operations.hpp>

auto process_event(sf::Event& window_event) {
	if (window_event.type == sf::Event::Closed)
		return false;
	return true;
}

bool try_place(auto& grid, auto const& word, auto const& font) {
	grid.emplace_back().setString(word);
	grid.back().setFont(font);
	grid.back().setCharacterSize(36);
	//grid.back().setFillColor(sf::Color::Black);
	grid.back().setPosition(15, (36 * (grid.size() - 1)) + 1);
	return true;
}

void makePlayWheelBackground(auto& playWheelBG) {
	playWheelBG.setRadius(100.f);
	playWheelBG.setPointCount(45);
	playWheelBG.setOrigin(100.f, 100.f);
	playWheelBG.setFillColor(sf::Color(255, 0, 0, 75));
	playWheelBG.setPosition(sf::Vector2f(800 / 2, 600 - 105));
}

int main() {
	// LOAD RESOURCES
	sf::Font font;
	font.loadFromFile("../font.ttf");

	// DEFINE DATA STRUCTURES
	std::vector<sf::Text> crossword_words;
	std::vector<sf::Text> letters_to_play;
	std::vector<std::pair<std::wstring, std::wstring>> word_def_map;

	games::loadVocabularyFromFile("../vocab.txt", word_def_map);

	std::ranges::sort(word_def_map, [](auto& lft, auto& rht)->bool {
		return lft.first.length() >= rht.first.length();
		});

	auto word = word_def_map[0].first;

	for (auto& word : std::ranges::filter_view(word_def_map, [&word](auto const& candidate_pair)->bool {
						return games::canSpell(word, candidate_pair.first);
						})) {
		if (!try_place(crossword_words, word.first, font))
			break;
	}

	sf::CircleShape letters_bg;
	makePlayWheelBackground(letters_bg);
	games::createPlayWheel(word, letters_to_play, letters_bg.getPosition(), 75.f, font);

	sf::RenderWindow game_window(sf::VideoMode(800, 600), "Language Games");
	while (game_window.isOpen()) {
		sf::Event window_event;
		while (game_window.pollEvent(window_event)) {
			if (!process_event(window_event))
				game_window.close();
		}

		game_window.clear();
		game_window.draw(letters_bg);

		std::ranges::for_each(crossword_words, [&game_window](auto const& word) {
			game_window.draw(word);
			});

		std::ranges::for_each(letters_to_play, [&game_window](auto const& c) {
			game_window.draw(c);
			});

		game_window.display();
	}

	return 0;
}