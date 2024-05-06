project(language-games)

find_library(SFML_AUDIO_DEBUG_DIR sfml-audio-d "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_GRAPHICS_DEBUG_DIR sfml-graphics-d "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_NETWORK_DEBUG_DIR sfml-network-d "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_SYSTEM_DEBUG_DIR sfml-system-d "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_WINDOW_DEBUG_DIR sfml-window-d "${CMAKE_CURRENT_SOURCE_DIR}/sfml")

find_library(SFML_AUDIO_DIR sfml-audio "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_GRAPHICS_DIR sfml-graphics "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_WINDOW_DIR sfml-network "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_SYSTEM_DIR sfml-system "${CMAKE_CURRENT_SOURCE_DIR}/sfml")
find_library(SFML_NETWORK_DIR sfml-window "${CMAKE_CURRENT_SOURCE_DIR}/sfml")

target_include_directories(
	app
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)