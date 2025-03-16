// GraphEngine.cpp : Defines the entry point for the application.

#include "OpinionSimulation.h"
#include "GraphGeneration.h"

int main()
{
    std::vector<std::vector<int>> adjacencyMatrix = GraphGeneration::GenerateRandomGraph(800, 0.002, 4);
    Graph graph{ adjacencyMatrix };
    OpinionSimulation simulation{ &graph };

    simulation.run();
}