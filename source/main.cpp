#include <chrono>
#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <file_operations.hpp>
#include <string_operations.hpp>

auto process_event(sf::Event& window_event) {
	if (window_event.type == sf::Event::Closed)
		return false;
	return true;
}

void createPlayWheel(auto const& characters, auto& letters_to_play, auto& center, float distance, auto& font) {
	float rotationOffset = 0;
	if (characters.length() != 4)
		rotationOffset = 90.0f + (360.0f / characters.length()) / 2.0f;

	for (int i = 0; i < characters.length(); ++i) {
		float angle = i * (360.0f / characters.length()) + rotationOffset;
		float radians = angle * (3.14159f / 180.0f);

		float offsetX = distance * std::cos(radians);
		float offsetY = distance * std::sin(radians);

		letters_to_play.emplace_back();

		letters_to_play.back().setFont(font);
		letters_to_play.back().setCharacterSize(42);
		letters_to_play.back().setString(characters[i]);

		sf::FloatRect textBounds = letters_to_play.back().getLocalBounds();
		letters_to_play.back().setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
		letters_to_play.back().setPosition(center.x + offsetX, center.y + offsetY);
	}
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
	auto can_be_spelled = std::ranges::filter_view(word_def_map, [&word](auto const& candidate_pair)->bool {
		return games::canSpell(word, candidate_pair.first);
		});

	for (auto& word : can_be_spelled) {
		if (!try_place(crossword_words, word.first, font))
			break;
	}

	sf::CircleShape letters_bg;
	makePlayWheelBackground(letters_bg);
	createPlayWheel(word, letters_to_play, letters_bg.getPosition(), 75.f, font);

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