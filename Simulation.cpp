#include "Simulation.h"
#include <iostream>

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
	else
	{
		initializeInfoText();
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
}

void Simulation::start()
{
	// THIS FUNCTION IS FOR INITIALIZING THE SIMULATION.

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
		m_graph->updateGeometry(m_nodeHeld, m_injectMousePos);

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
				text.setCharacterSize(Settings::FONT_SIZE);
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
	//---------------- INJECT ALGORITHMS BELOW THIS LINE. ----------------


	//---------------- INJECT ALGORITHMS ABOVE THIS LINE. ----------------

	m_currentTimeStep += 1;
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
		<< "Persuasion: " << "????????????????????????" << "\n"
		<< "Gullability: " << "????????????????????????" << "\n";
}

void Simulation::injectInfoTextUpdate(int nodeIndex, std::stringstream& ss)
{
	Node& node = m_graph->getNode(nodeIndex);

	ss << "Selected Node: " << nodeIndex << "\n"
		<< "Persuasion: " << node.persuation << "\n"
		<< "Gullability: " << node.gullability << "\n";
}