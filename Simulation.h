#pragma once 

#include <SFML/Window.hpp>
#include "Input.h"
#include <cstdint>
#include <map>

class Simulation
{
public:
	Simulation(Graph* graph, const std::string& fontPath = "../../../fonts/nasa.ttf");

	virtual ~Simulation() = default;

	void run();
	void stop()
	{
		m_isRunning = false;
	}
	void pause()
	{
		m_isPaused = true;
	}
	void unpause()
	{
		m_isPaused = false;
	}

	// For logging user-defined numeric data as the simulation progresses.
	template<typename T>
	void recordValue(const std::string& varName, T value)
	{
		m_trackedData[varName].push_back(static_cast<double>(value));
	}
	void exportTractedDataToCSV(const std::string& fileName) const;

protected:
	virtual void onStart();
	virtual void onStep();
	virtual void injectInputHandling();
	virtual void injectInfoTextInitialization(std::stringstream& ss);
	virtual void injectInfoTextUpdate(int nodeIndex, std::stringstream& ss);

protected:
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
	bool m_isPaused = false;
	uint64_t m_currentTimeStep = 0;

private:
	void start();
	void step();
	void handleInputs();
	void initializeInfoText();
	void setInfoText();

	std::map<std::string, std::vector<double>> m_trackedData;
};