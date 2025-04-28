#pragma once 

#include <vector>

namespace GraphGeneration
{
	extern std::vector<std::vector<int>> GenerateRandomGraph(unsigned int numOfNodes, float connectionProbability, int maxStrength);
	extern std::vector<std::vector<int>> GenerateWattsStrogatzModel(unsigned int numOfNodes, unsigned int meanDegree, float rewireProbability, int maxStrength);

}