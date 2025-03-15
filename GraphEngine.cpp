// GraphEngine.cpp : Defines the entry point for the application.

#include "Simulation.h"

int main()
{
    Graph graph{ "../../../testMatrix.txt" };
    Simulation simulation{ &graph };

    simulation.run();
}