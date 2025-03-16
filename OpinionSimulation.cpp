#include "OpinionSimulation.h"

#include <iostream>

void OpinionSimulation::start()
{
	using Simulation::start;
	// Optionally, create a random device to seed the RNG.
	// For now, I use fixed seeding for reproducibility.

	rngEngine.seed(0);

	std::uniform_int_distribution<> intDist(1, 10);
	std::uniform_int_distribution<> broadcastFreqDist(0, 1000);

	// Initialize the node variations.
	// Maybe you can manually insert personalities later, for now it is random.
	for (size_t i = 0; i < m_graph->getNumOfNodes(); ++i)
	{
		Node& node = m_graph->getNode(i);
		node.gullability = intDist(rngEngine);
		node.persuation = intDist(rngEngine);
		node.opinionStrength = intDist(rngEngine);
		node.broadcastFrequency = broadcastFreqDist(rngEngine);

		int opinionDeterminer = intDist(rngEngine);
		if (opinionDeterminer > 5)
		{
			node.opinion = false;
			m_graph->setNodeColor(i, sf::Color::Blue);
		}

		if (node.opinion)
		{
			m_graph->setNodeColor(i, sf::Color::Red);
		}
	}
}

void OpinionSimulation::injectInfoTextInitialization(std::stringstream& ss)
{
	std::cout << "Hello" << std::endl;
	ss << "Selected Node: " << "????????????????????????" << "\n"
		<< "Opinion: " << "????????????????????????" << "\n"
		<< "Opinion Strength: " << "????????????????????????" << "\n"
		<< "Persuasion: " << "????????????????????????" << "\n"
		<< "Gullability: " << "????????????????????????" << "\n"
		<< "Broadcast Frequency: " << "????????????????????????" << "\n";
}