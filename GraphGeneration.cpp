#include "GraphGeneration.h"

#include <random>

namespace GraphGeneration
{
	std::vector<std::vector<int>> GenerateRandomGraph(unsigned int numOfNodes, float connectionProbability, int maxStrength)
	{
		std::random_device rd;
		std::mt19937 rngEngine(rd());
		std::uniform_real_distribution<> realDist(0, 1);

		// std::vector of size numOfNodes, where each element is another std::vector<int> of size numOfNodes, initialized to 0.
		std::vector<std::vector<int>> adjacencyMatrix(numOfNodes, std::vector<int>(numOfNodes, 0));

		for (size_t i = 0; i < numOfNodes; ++i)
		{
			for (size_t j = 0; j < numOfNodes; ++j)
			{
				if (i != j)
				{
					float existenceDeterminer = realDist(rngEngine);
					if (existenceDeterminer < connectionProbability)
					{
						float weightDeterminer = realDist(rngEngine);
						adjacencyMatrix[i][j] = int(realDist(rngEngine) * float(maxStrength) + 1.0f);
					}
				}
			}
		}

		return adjacencyMatrix;
	}
}