#pragma once 

#include <vector>

namespace GraphGeneration
{
	extern std::vector<std::vector<int>> GenerateRandomGraph(unsigned int numOfNodes, float connectionProbability, int maxStrength);
}