#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

auto process_event(sf::Event& window_event) {
	if (window_event.type == sf::Event::Closed)
		return false;
	return true;
}

struct Word {
	std::wstring 
};

int main() {
	sf::RenderWindow game_window(sf::VideoMode(800, 600), "Language Games");

	sf::Font testing_font;
	testing_font.loadFromFile("../font.ttf");

	sf::Text testing_text;
	testing_text.setFont(testing_font);
	testing_text.setString(L"привіт");

	while (game_window.isOpen()) {

		sf::Event window_event;
		while (game_window.pollEvent(window_event)) {
			if (!process_event(window_event))
				game_window.close();
		}

		game_window.clear();
		game_window.draw(testing_text);
		game_window.display();

	}

	return 0;
}