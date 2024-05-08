#include <game_loop.hpp>

int main() {
	// INITIALIZE GAME RESOURCES
	auto shared_resources = std::make_unique<games::SharedResources>();
	games::loadSharedResources(shared_resources);

	auto level_resources = std::make_unique<games::LevelResources>();

	// INITIALIZE SFML RENDER WINDOW & RUN GAME LOOP
	sf::RenderWindow game_window(sf::VideoMode(800, 600), "Language Games");

	while (game_window.isOpen()) {
		if (games::loadLevelResources(shared_resources, level_resources)) {
			games::runGameLoop(game_window, shared_resources, level_resources);
			games::cleanUpLevelResources(level_resources, shared_resources->level++);
		}
		else {
			std::cerr << "Failed to load resources for level " << shared_resources->level << ". Exiting.\n";
			game_window.close();
		}
	}

	return 0;
}