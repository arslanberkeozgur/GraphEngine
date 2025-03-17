#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// TODO: For now, using constexpr means these settings CANNOT BE CHANGED IN RUNTIME. 
// If you want them to be modified during runtime, rename them as extern, remove their given values here, 
// create a file Settings.cpp, and define them there in the same namespace.

namespace Settings
{
	// ---------- WINDOW & VIEW ----------
	constexpr bool FULLSCREEN = true;
	constexpr unsigned int WIDTH = 800; 	// If not fullscreen, set the width and height.
	constexpr unsigned int HEIGHT = 600;
	constexpr unsigned int FRAMERATE_LIMIT = 60; 	// If 0, no limit is imposed. (No limit is better for algorithms, bad for visualization).
	constexpr float ORIGINAL_VIEW_SIZE = 600.0f; 	// Starting view size in world coordinates. (Can be changed by zooming)

	// ---------- VISUAL -----------
	constexpr int ANTIALIASING = 4;	// Better looks, but performance heavy.
	constexpr sf::Color BACKGROUND_COLOR = sf::Color::Black;
	constexpr sf::Color NODE_COLOR = sf::Color::Red;
	constexpr sf::Color EDGE_COLOR = sf::Color::White;
	constexpr sf::Color EDGE_ARROW_COLOR = sf::Color::Red;
	constexpr float EDGE_ALPHA = 0.4f;
	constexpr float NODE_SIZE = 3.0f;
	constexpr float EDGE_THICKNESS = 0.4f;
	constexpr float ATTRACTION_FORCE = 0.0005f;    // Not increasing this above 0.1-0.2 is recommended. Otherwise exhibits weird behavior.
	constexpr float REPULSION_FORCE = 0.8f;      // Same recommendation as above. 
	constexpr float EDGE_ARROW_SIZE = 0.4f;

	// ---------- TEXT ----------
	constexpr unsigned int WEIGHT_FONT_SIZE = 8;
	constexpr unsigned int FONT_SIZE = 24;
	constexpr sf::Color FONT_COLOR = sf::Color::White;

	// ---------- SIMULATION ----------
	constexpr unsigned int NUMBER_OF_PHYSICS_ITERATIONS = 1000; // Number of physics iterations ran before the simulation begins.
	constexpr bool DYNAMIC_PHYSICS = true; // If you set this to false, I recommend increasing the NUMBER_OF_PHYSICS_ITERATIONS.
	constexpr bool SHOW_WEIGHTS = false;		// Performance intensive for large graphs.
	constexpr float WEIGHT_TEXT_DISTANCE = 0.1f; // Positioning of weight indicators from edges.
	constexpr bool DISPLAY_NODE_INFO = true;
}