project(language-games)

SET (APP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
SET (EXTERNALS_DIR "${CMAKE_CURRENT_SOURCE_DIR}../external")
SET (SFML_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}../external/sfml")
SET (INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}../include")

find_library(SFML_AUDIO_DEBUG_DIR sfml-audio-d ${SFML_BUILD_DIR})
find_library(SFML_GRAPHICS_DEBUG_DIR sfml-graphics-d ${SFML_BUILD_DIR})
find_library(SFML_NETWORK_DEBUG_DIR sfml-network-d ${SFML_BUILD_DIR})
find_library(SFML_SYSTEM_DEBUG_DIR sfml-system-d ${SFML_BUILD_DIR})
find_library(SFML_WINDOW_DEBUG_DIR sfml-window-d ${SFML_BUILD_DIR})

find_library(SFML_AUDIO_DIR sfml-audio ${SFML_BUILD_DIR})
find_library(SFML_GRAPHICS_DIR sfml-graphics ${SFML_BUILD_DIR})
find_library(SFML_WINDOW_DIR sfml-network ${SFML_BUILD_DIR})
find_library(SFML_SYSTEM_DIR sfml-system ${SFML_BUILD_DIR})
find_library(SFML_NETWORK_DIR sfml-window ${SFML_BUILD_DIR})

# app
add_executable(
	app
	${APP_SOURCE_DIR}/main.cpp
)

target_link_libraries(
	app
	PRIVATE optimized ${SFML_AUDIO_DIR}
	PRIVATE optimized ${SFML_SYSTEM_DIR}
	PRIVATE optimized ${SFML_WINDOW_DIR}
	PRIVATE optimized ${SFML_NETWORK_DIR}
	PRIVATE optimized ${SFML_GRAPHICS_DIR}
	
	PRIVATE debug ${SFML_AUDIO_DEBUG_DIR}
	PRIVATE debug ${SFML_SYSTEM_DEBUG_DIR}
	PRIVATE debug ${SFML_WINDOW_DEBUG_DIR}
	PRIVATE debug ${SFML_NETWORK_DEBUG_DIR}
	PRIVATE debug ${SFML_GRAPHICS_DEBUG_DIR}
)

target_include_directories(
	app
	PRIVATE ${EXTERNALS_DIR}
    PRIVATE ${INCLUDE_DIR}
)

set_property(
    TARGET app
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${APP_SOURCE_DIR}"
)