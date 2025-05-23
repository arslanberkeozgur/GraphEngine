#include "Simulation.h"
#include <iostream>
#include <fstream>

Simulation::Simulation(Graph* graph, const std::string& fontPath)
	: m_graph{ graph }
{
	if (Settings::ANTIALIASING > 0)
	{
		m_contextSettings.antiAliasingLevel = Settings::ANTIALIASING;
	}

	if (Settings::FULLSCREEN)
	{
		sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
		m_aspectRatio = float(desktopMode.size.y) / desktopMode.size.x;

		if (desktopMode.isValid())
		{
			// TODO: Set window decorations off for fullscreen, in Windows they do not behave.
			m_window.create(desktopMode, "Graph Engine", sf::State::Fullscreen, m_contextSettings);
		}
		else
		{
			m_window.create(desktopMode, "Graph Engine", sf::State::Windowed, m_contextSettings);
		}
	}
	else
	{
		m_aspectRatio = float(Settings::HEIGHT) / Settings::WIDTH;
		m_window.create(sf::VideoMode{ {Settings::WIDTH, Settings::HEIGHT} }, "Graph Engine");
	}

	if (Settings::FRAMERATE_LIMIT > 0)
	{
		m_window.setFramerateLimit(Settings::FRAMERATE_LIMIT);
	}

	if (m_aspectRatio <= 0)
	{
		std::cerr << "ERROR::INVALID ASPECT RATIO" << std::endl;
	}

	m_view.setCenter({ 0, 0 });
	m_view.setSize({ Settings::ORIGINAL_VIEW_SIZE, m_aspectRatio * Settings::ORIGINAL_VIEW_SIZE });
	m_window.setView(m_view);

	bool fontLoaded = m_font.openFromFile(fontPath);
	if (!fontLoaded)
	{
		std::cerr << "WARNING::FONT NOT LOADED, EXPECT UNDEFINED BEHAVIOR" << std::endl;
	}
}

void Simulation::handleInputs()
{
	// This should exist in all overrides.
	Input::HandleInputs(m_window, m_inputState, m_view, *m_graph);

	// Scrolling
	if (m_inputState.mouseLeftPressed)
	{
		sf::Vector2f potentialCenter = m_view.getCenter() - m_inputState.deltas.deltaMouseWorld;
		float halfX = m_view.getSize().x / 2;
		float halfY = m_view.getSize().y / 2;

		m_view.move(-m_inputState.deltas.deltaMouseWorld);
		m_window.setView(m_view);

		// You need to remap the current mouse world position after changing view: coordinates have changed!
		// IMPORTANT: After each view change, call a remapping function for all coordinates.
		m_inputState.lastMouseWorldPos = m_window.mapPixelToCoords(m_inputState.lastMousePos);

	}
	m_nodeHeld = -1;
	if (m_inputState.mouseRightPressed && m_inputState.selectedNodeIndexForMoving >= 0)
	{
		m_nodeHeld = m_inputState.selectedNodeIndexForMoving;
		m_injectMousePos = m_inputState.lastMouseWorldPos;
	}

	injectInputHandling();
}

void Simulation::start()
{
	initializeInfoText();
	onStart();
}

void Simulation::onStart()
{
	// ----YOU WILL INJECT YOUR INITIALIZATION CODE BELOW----

	// ----YOU WILL INJECT YOUR INITIALIZATION CODE ABOVE----
}

void Simulation::injectInputHandling()
{
	// ----YOU WILL INJECT YOUR INPUT HANDLING CODE BELOW----

	// ----YOU WILL INJECT YOUR INPUT HANDLING CODE ABOVE----
}

// Render/simulation loop.
void Simulation::run()
{
	start();
	while (m_window.isOpen())
	{
		handleInputs();
		if (m_isRunning && !m_isPaused)
		{
			step();
		}
		if (Settings::DYNAMIC_PHYSICS)
		{
			m_graph->updateGeometry(m_nodeHeld, m_injectMousePos);
		}

		m_window.clear(Settings::BACKGROUND_COLOR);
		m_window.draw(*m_graph);

		// Displaying node info
		if (Settings::DISPLAY_NODE_INFO)
		{
			sf::View originalView = m_window.getView();
			sf::View textView = m_window.getDefaultView();
			m_window.setView(textView);
			m_window.draw(*m_infoText);
			m_window.setView(originalView);
			setInfoText();
		}

		if (Settings::SHOW_WEIGHTS)
		{
			sf::View originalView = m_window.getView();
			sf::View textView = m_window.getDefaultView();
			m_window.setView(textView);
			for (size_t i = 0; i < m_graph->getNumOfEdges(); ++i)
			{

				Edge& edge = m_graph->getEdge(i);
				sf::Text text(m_font);
				text.setString(std::to_string(edge.weight));
				text.setCharacterSize(Settings::WEIGHT_FONT_SIZE);
				text.setFillColor(Settings::FONT_COLOR);

				sf::Vector2f worldPos = edge.position + Settings::WEIGHT_TEXT_DISTANCE * edge.normal;
				sf::Vector2i pixelPos = m_window.mapCoordsToPixel(worldPos, originalView);
				sf::Vector2f pixelPosFloat = static_cast<sf::Vector2f>(pixelPos);

				text.setPosition(pixelPosFloat);
				m_window.draw(text);
			}
			m_window.setView(originalView);
		}

		m_window.display();

		Input::ResetDeltas(m_inputState);
	}
}

void Simulation::step()
{
	onStep();
	m_currentTimeStep += 1;
}

void Simulation::onStep()
{
	// THIS IS WHERE YOU INJECT YOUR UPDATE CODE.
}

void Simulation::initializeInfoText()
{
	m_infoText.emplace(m_font);
	m_infoText->setCharacterSize(Settings::FONT_SIZE);
	m_infoText->setFillColor(Settings::FONT_COLOR);
	sf::Vector2u windowSize = m_window.getSize();
	std::stringstream ss;

	// -----BELOW IS WHERE YOU INJECT YOUR TEXT-----

	injectInfoTextInitialization(ss);

	// -----ABOVE IS WHERE YOU INJECT YOUR TEXT-----

	m_infoText->setString(ss.str());
	sf::FloatRect textBounds = m_infoText->getLocalBounds();
	m_infoText->setOrigin({ textBounds.size.x, 0.f });
	m_infoText->setPosition({ static_cast<float>(windowSize.x) - 10.f, 10.f });
}

void Simulation::setInfoText()
{
	if (m_inputState.selectedNodeIndexForInfo >= 0)
	{
		int i = m_inputState.selectedNodeIndexForInfo;
		std::stringstream ss;

		// -----BELOW IS WHERE YOU INJECT YOUR TEXT-----

		injectInfoTextUpdate(i, ss);

		// -----ABOVE IS WHERE YOU INJECT YOUR TEXT-----

		m_infoText->setString(ss.str());
	}
}

void Simulation::injectInfoTextInitialization(std::stringstream& ss)
{
	ss << "Selected Node: " << "????????????????????????" << "\n"
		<< "Minimum Degree: " << "????????????????????????" << "\n"
		<< "Average Degree: " << "????????????????????????" << "\n"
		<< "Minimum In-Degree: " << "????????????????????????" << "\n"
		<< "Minimum Out-Degree: " << "????????????????????????" << "\n"
		<< "Average In-Degree: " << "????????????????????????" << "\n"
		<< "Average Out-Degree: " << "????????????????????????" << "\n"
		<< "Maximum Degree: " << "????????????????????????" << "\n"
		<< "Maximum In-Degree: " << "????????????????????????" << "\n"
		<< "Maximum Out-Degree: " << "????????????????????????" << "\n";
}

void Simulation::injectInfoTextUpdate(int nodeIndex, std::stringstream& ss)
{
	Node& node = m_graph->getNode(nodeIndex);

	ss << "Selected Node: " << nodeIndex << "\n"
		<< "Minimum Degree: " << m_graph->getMinimumDegree() << "\n"
		<< "Average Degree: " << m_graph->getAverageDegree() << "\n"
		<< "Minimum In-Degree: " << m_graph->getMinimumInDegree() << "\n"
		<< "Minimum Out-Degree: " << m_graph->getMinimumOutDegree() << "\n"
		<< "Average In-Degree: " << m_graph->getAverageInDegree() << "\n"
		<< "Average Out-Degree: " << m_graph->getAverageOutDegree() << "\n"
		<< "Maximum Degree: " << m_graph->getMaximumDegree() << "\n"
		<< "Maximum In-Degree: " << m_graph->getMaximumInDegree() << "\n"
		<< "Maximum Out-Degree: " << m_graph->getMaximumOutDegree() << "\n";
}

void Simulation::exportTractedDataToCSV(const std::string& fileName) const
{
	std::ofstream ofs(fileName.c_str());
	if (!ofs.is_open())
	{
		std::cerr << "ERROR::FAILED TO OPEN FILE " << fileName << " FOR WRITING" << std::endl;
		return;
	}

	// Write header row
	ofs << "Index";
	for (std::map<std::string, std::vector<double>>::const_iterator it = m_trackedData.begin();
		it != m_trackedData.end(); ++it)
	{
		ofs << "," << it->first;
	}
	ofs << "\n";

	// Determine the maximum number of samples among all tracked variables
	size_t maxLength = 0;
	for (std::map<std::string, std::vector<double> >::const_iterator it = m_trackedData.begin();
		it != m_trackedData.end(); ++it)
	{
		if (it->second.size() > maxLength)
		{
			maxLength = it->second.size();
		}
	}

	// Write data rows
	for (size_t i = 0; i < maxLength; ++i)
	{
		ofs << i;  // Write the index in the first column
		for (std::map<std::string, std::vector<double> >::const_iterator it = m_trackedData.begin();
			it != m_trackedData.end(); ++it)
		{
			// If this variable has a value at row i, write it; otherwise leave blank
			if (i < it->second.size())
			{
				ofs << "," << it->second[i];
			}
			else
			{
				ofs << ",";
			}
		}
		ofs << "\n";
	}

	ofs.close();
	std::cout << "Tracked data successfully exported to " << fileName << std::endl;
}