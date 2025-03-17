// GraphEngine.cpp : Defines the entry point for the application.

#include "OpinionSimulation.h"
#include "GraphGeneration.h"

int main()
{
    std::vector<std::vector<int>> adjacencyMatrix = GraphGeneration::GenerateRandomGraph(600, 0.005, 4);
    Graph graph{ adjacencyMatrix };
    OpinionSimulation simulation{ &graph };

    simulation.run();
}