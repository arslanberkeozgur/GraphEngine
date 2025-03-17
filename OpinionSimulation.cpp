#include "OpinionSimulation.h"

#include <iostream>

void OpinionSimulation::onStart()
{
	// Optionally, create a random device to seed the RNG.
	// For now, I use fixed seeding for reproducibility.
	std::random_device rd;
	rngEngine.seed(rd());

	std::uniform_int_distribution<> intDist(1, 10);
	std::uniform_int_distribution<> broadcastFreqDist(20, 100);

	// Initialize the node variations.
	// Maybe you can manually insert personalities later, for now it is random.
	for (size_t i = 0; i < m_graph->getNumOfNodes(); ++i)
	{
		Node& node = m_graph->getNode(i);

		node.gullability = intDist(rngEngine);
		node.persuasion = intDist(rngEngine);
		node.opinionStrength = intDist(rngEngine);
		node.broadcastFrequency = broadcastFreqDist(rngEngine);

		int opinionDeterminer = intDist(rngEngine);
		if (opinionDeterminer <= 5)
		{
			node.opinion = true;
			m_graph->setNodeColor(i, trueOpinionColor);
			numOfTrueNodes++;
		}
		else
		{
			node.opinion = false;
			m_graph->setNodeColor(i, falseOpinionColor);
			numOfFalseNodes++;
		}
	}
}

void OpinionSimulation::onStep()
{
	if (m_currentTimeStep % 20 == 0)
	{
		unflashEdges();
	}
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
				int interactionStrength = node.persuasion * endNode->gullability * connection->weight;

				// Successful persuasion
				if (successDeterminer < interactionStrength)
				{
					// Flash the connection.
					flashingEdges.push_back(connection);
					if (endNode->opinion == node.opinion)
					{
						endNode->opinionStrength += node.persuasion;
					}
					else
					{
						if (endNode->opinionStrength > node.persuasion)
						{
							endNode->opinionStrength -= node.persuasion;
						}
						else
						{
							endNode->opinion = !endNode->opinion;
							int excessOpinion = node.persuasion - endNode->opinionStrength;
							endNode->opinionStrength = excessOpinion;

							if (node.opinion)
							{
								numOfTrueNodes++;
								numOfFalseNodes--;
							}
							else
							{
								numOfTrueNodes--;
								numOfFalseNodes++;
							}

							if (endNode->opinion)
							{
								m_graph->setNodeColor(endNode->index, trueOpinionColor);
							}
							else
							{
								m_graph->setNodeColor(endNode->index, falseOpinionColor);
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
		m_graph->setEdgeColor(flashingEdges[i]->index, sf::Color::Yellow);
	}
}

void OpinionSimulation::unflashEdges()
{
	for (int i = 0; i < flashingEdges.size(); ++i)
	{
		m_graph->setEdgeColor(flashingEdges[i]->index, sf::Color::White, Settings::EDGE_ALPHA);
	}
	flashingEdges.clear();
}

void OpinionSimulation::injectInputHandling()
{
	if (m_inputState.keyPressed_P && !pKeyHeld)
	{
		m_isPaused = !m_isPaused;
	}
	pKeyHeld = m_inputState.keyPressed_P;
}

void OpinionSimulation::injectInfoTextInitialization(std::stringstream& ss)
{
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
		<< "Persuasion: " << node.persuasion << "\n"
		<< "Gullability: " << node.gullability << "\n"
		<< "Broadcast Frequency: " << node.broadcastFrequency << "\n";
}