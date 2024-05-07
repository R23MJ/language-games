#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
#include <chrono>
#include <random>
#include <ranges>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <codecvt>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

auto process_event(sf::Event& window_event) {
	if (window_event.type == sf::Event::Closed)
		return false;
	return true;
}

bool loadVocabularyFromFile(auto& file_name, auto& word_definitions) {
	std::wifstream file(file_name); // Adjust file path as needed
	file.imbue(std::locale(file.getloc(), new std::codecvt_utf8_utf16<wchar_t>)); // Set file stream to use UTF-8 encoding
	if (!file.is_open()) {
		std::cerr << "Error: Unable to open file.\n";
		return 1;
	}

	std::wstring line;
	while (std::getline(file, line)) {
		std::wistringstream iss(line);
		std::wstring targetWord, englishDefinition;

		if (iss >> targetWord) {
			std::getline(iss, englishDefinition);
			word_definitions[targetWord] = englishDefinition;
		}
	}

	file.close();
}

void getWord(auto& word_list, auto& word, int length) {
	auto it = std::ranges::find_if(word_list, [=](const auto& pair) {
		return pair.first.length() == length;
		});
	word = it != word_list.end() ? it->first : L"";
}

bool canSpell(auto letters, auto& word) {
	return std::ranges::all_of(word, [&letters](auto const& c)->bool {
		if (letters.find(c) == letters.end() || letters[c] == 0)
			return false;

		--letters[c];
		return true;
		});
}

void fillFromString(auto& container, auto& letters) {
	std::ranges::for_each(letters, [&container](auto& c) {
		if (container.find(c) == container.end())
			container.emplace(std::make_pair(c, 0));

		container[c]++;
		});
}

void createTexts(auto const& characters, auto& texts, auto& center, float distance, auto& font) {
	// Set rotation offset for even distribution
	float rotationOffset = 0;
	if (characters.length() != 4)
		rotationOffset = 90.0f + (360.0f / characters.length()) / 2.0f;

	// Place characters evenly spaced in a circle around the center point
	for (int i = 0; i < characters.length(); ++i) {
		// Calculate angle for current text with rotation offset
		float angle = i * (360.0f / characters.length()) + rotationOffset;
		// Convert angle to radians
		float radians = angle * (3.14159f / 180.0f);
		// Calculate position offsets based on angle and distance
		float offsetX = distance * std::cos(radians);
		float offsetY = distance * std::sin(radians);

		texts[i].setFont(font);
		texts[i].setCharacterSize(42);
		texts[i].setString(characters[i]);

		sf::FloatRect textBounds = texts[i].getLocalBounds();
		texts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
		texts[i].setPosition(center.x + offsetX, center.y + offsetY);
	}
}

int main() {
	std::unordered_map<std::wstring, std::wstring> word_definitions;
	loadVocabularyFromFile("../vocab.txt", word_definitions);

	std::wstring word;
	getWord(word_definitions, word, 5);

	std::unordered_map<wchar_t, int> letters;
	fillFromString(letters, word);

	auto filtered_view = std::ranges::views::filter(word_definitions, [&letters](auto const& word) {
		return canSpell(letters, word.first);
		});

	sf::CircleShape letters_bg(100.f, 45);
	letters_bg.setOrigin(100.f, 100.f);
	letters_bg.setFillColor(sf::Color(255, 0, 0, 75));
	letters_bg.setPosition(sf::Vector2f(800 / 2, 600 - 105));

	sf::Font font;
	font.loadFromFile("../font.ttf");
	std::vector<sf::Text> texts(word.length());

	createTexts(word, texts, letters_bg.getPosition(), 75.f, font);

	sf::RenderWindow game_window(sf::VideoMode(800, 600), "Language Games");
	while (game_window.isOpen()) {
		sf::Event window_event;
		while (game_window.pollEvent(window_event)) {
			if (!process_event(window_event))
				game_window.close();
		}

		game_window.clear();
		game_window.draw(letters_bg);

		std::ranges::for_each(texts, [&game_window](auto const& c) {
			game_window.draw(c);
			});

		game_window.display();
	}

	return 0;
}