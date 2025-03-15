#pragma once 

#include <SFML/Window.hpp>
#include "Input.h"
#include <cstdint>

class Simulation
{
public:
	Simulation(Graph* graph, const std::string& fontPath = "../../../nasa.ttf");

	void start();
	void run();
	void step();
	void stop()
	{
		m_isRunning = false;
	}

	void handleInputs();
	void setInfoText();

private:
	sf::RenderWindow m_window;
	sf::View m_view;
	float m_aspectRatio = 0;
	sf::ContextSettings m_contextSettings;

	Input::InputState m_inputState;
	sf::Font m_font;
	std::optional<sf::Text> m_infoText;

	int m_nodeHeld = -1;
	sf::Vector2f m_injectMousePos = { 0.0f, 0.0f };

	Graph* m_graph;

	bool m_isRunning = true;
	uint64_t m_currentTimeStep = 0;
};