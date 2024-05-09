#include <game_loop.hpp>

int main() {
	auto shared_resources = std::make_unique<games::SharedResources>();
	auto level_resources = std::make_unique<games::LevelResources>();
	games::loadSharedResources(shared_resources);

	while (shared_resources->game_window.isOpen()) {
		if (games::loadLevelResources(shared_resources, level_resources)) {
			games::runGameLoop(shared_resources, level_resources);
			games::cleanUpLevelResources(level_resources, shared_resources->level++);
		}
		else {
			std::cerr << "Failed to load resources for level " << shared_resources->level << ". Exiting.\n";
			shared_resources->game_window.close();
		}
	}

	return 0;
}