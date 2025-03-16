#include "OpinionSimulation.h"

#include <iostream>

void OpinionSimulation::onStart()
{
	flashingEdges.reserve(100);

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

void OpinionSimulation::step()
{
	unflashEdges();
	for (size_t i = 0; i < m_graph->getNumOfNodes(); ++i)
	{
		Node& node = m_graph->getNode(i);

		if (m_currentTimeStep % node.broadcastFrequency == 0)
		{
			for (int j = 0; j < node.outEdges.size(); ++j)
			{
				Edge* connection = node.outEdges[j];
				Node* endNode = connection->end;

				int successDeterminer = interactionDistribution(rngEngine);
				int interactionStrength = node.persuation * endNode->gullability * connection->weight;

				// Successful persuasion
				if (successDeterminer < interactionStrength)
				{
					// Flash the connection.
					flashingEdges.push_back(connection);
					if (endNode->opinion == node.opinion)
					{
						//endNode->opinionStrength += node.persuation;
					}
					else
					{
						if (endNode->opinionStrength > node.persuation)
						{
							endNode->opinionStrength -= node.persuation;
						}
						else
						{
							endNode->opinion = !endNode->opinion;
							int excessOpinion = node.persuation - endNode->opinionStrength;
							endNode->opinionStrength = excessOpinion;

							if (endNode->opinion)
							{
								m_graph->setNodeColor(i, sf::Color::Red);
							}
							else
							{
								m_graph->setNodeColor(i, sf::Color::Blue);
							}
						}
					}
				}
			}
		}
	}

	flashEdges();
}

void OpinionSimulation::flashEdges()
{
	for (int i = 0; i < flashingEdges.size(); ++i)
	{
		m_graph->setEdgeColor(flashingEdges[i]->index, sf::Color::Green);
	}
}

void OpinionSimulation::unflashEdges()
{
	for (int i = 0; i < flashingEdges.size(); ++i)
	{
		m_graph->setEdgeColor(flashingEdges[i]->index, sf::Color::White);
	}
	flashingEdges.clear();
}

//void OpinionSimulation::handleInputs()
//{
//	if (m_inputState.keyPressed_P)
//	{
//		m_isPaused = true;
//	}
//	else
//	{
//		m_isPaused = false;
//	}
//}

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

void OpinionSimulation::injectInfoTextUpdate(int nodeIndex, std::stringstream& ss)
{
	Node& node = m_graph->getNode(nodeIndex);

	ss << "Selected Node: " << nodeIndex << "\n"
		<< "Opinion: " << node.opinion << "\n"
		<< "Opinion Strength: " << node.opinionStrength << "\n"
		<< "Persuasion: " << node.persuation << "\n"
		<< "Gullability: " << node.gullability << "\n"
		<< "Broadcast Frequency: " << node.broadcastFrequency << "\n";
}