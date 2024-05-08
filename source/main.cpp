#include <board_operations.hpp>
#include <file_operations.hpp>
#include <game_operations.hpp>
#include <string_operations.hpp>

bool loadSharedResources(std::unique_ptr<games::SharedResources>& resources) {
	bool loaded = true;
	loaded &= resources->alice.loadFromFile("../resources/Alice.ttf");
	loaded &= games::loadVocabularyFromFile("../resources/vocab.txt", resources->word_def_map);

	// SORT WORD MAP BY LENGTH OF WORD
	std::ranges::sort(resources->word_def_map, [](auto& lft, auto& rht)->bool {
		return lft.first.length() >= rht.first.length();
		});

	games::loadWheelBackground(resources->wheel_background, 100.f);

	return loaded;
}

bool loadLevelResources(std::unique_ptr<games::SharedResources> const& shared, std::unique_ptr<games::LevelResources>& resources, size_t level) {
	return true;
}

void cleanUpLevelResources(std::unique_ptr<games::LevelResources>& resources, size_t level) {

}

int main() {
	// INITIALIZE GAME RESOURCES
	auto shared_resources = std::make_unique<games::SharedResources>();
	loadSharedResources(shared_resources);

	auto level_resources = std::make_unique<games::LevelResources>();

	// INITIALIZE SFML RENDER WINDOW & RUN GAME LOOP
	sf::RenderWindow game_window(sf::VideoMode(800, 600), "Language Games");

	size_t level = 0;
	while (game_window.isOpen()) {
		if (loadLevelResources(shared_resources, level_resources, level)) {
			games::runGameLoop(game_window, shared_resources, level_resources);
			cleanUpLevelResources(level_resources, level++);
		}
		else {
			std::cerr << "Failed to load resources for level " << level << ". Exiting.\n";
			game_window.close();
		}
	}

	return 0;
}