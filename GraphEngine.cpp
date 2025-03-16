// GraphEngine.cpp : Defines the entry point for the application.

#include "Simulation.h"
#include "GraphGeneration.h"

int main()
{
    std::vector<std::vector<int>> adjacencyMatrix = GraphGeneration::GenerateRandomGraph(100, 0.01, 4);
    Graph graph{ adjacencyMatrix };
    Simulation simulation{ &graph };

    simulation.run();
}