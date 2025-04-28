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

	std::vector<std::vector<int>> GenerateWattsStrogatzModel(unsigned int numOfNodes, unsigned int meanDegree, float rewireProbability, int maxStrength)
	{
		if (meanDegree >= numOfNodes)
			throw std::invalid_argument("Mean Degree must be smaller than number of the nodes!");

		if (meanDegree % 2 != 0)
			throw std::invalid_argument("Mean Degree must be even!");

		std::random_device rd;
		std::mt19937 rngEngine(rd());
		std::uniform_real_distribution<> realDist(0, 1);
		std::uniform_int_distribution<> nodeDist(0, numOfNodes - 1);
		std::uniform_int_distribution<> weightDist(1, maxStrength);

		std::vector<std::vector<int>> adjacencyMatrix(numOfNodes, std::vector<int>(numOfNodes, 0));

		for (unsigned int i = 0; i < numOfNodes; i++)
		{
			for (unsigned int j = 1; j <= meanDegree / 2; j++)
			{
				unsigned int neighbor = (i + j) % numOfNodes;
				int weight = weightDist(rngEngine);
				adjacencyMatrix[i][neighbor] = weight;
				adjacencyMatrix[neighbor][i] = weight;
			}
		}

		for (unsigned int i = 0; i < numOfNodes; i++)
		{
			for (unsigned int j = 1; j <= meanDegree / 2; j++)
			{
				unsigned int neighbor = (i + j) % numOfNodes;

				if (realDist(rngEngine) < rewireProbability)
				{
					unsigned int newNeighbor;

					do
					{
						newNeighbor = nodeDist(rngEngine);
					} while (newNeighbor == i || adjacencyMatrix[i][newNeighbor] != 0);

					adjacencyMatrix[i][neighbor] = 0;
					adjacencyMatrix[neighbor][i] = 0;

					int weight = weightDist(rngEngine);
					adjacencyMatrix[i][newNeighbor] = weight;
					adjacencyMatrix[newNeighbor][i] = weight;

				}
			}
		}


		return adjacencyMatrix;
	}
}