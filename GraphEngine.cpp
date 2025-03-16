// GraphEngine.cpp : Defines the entry point for the application.

#include "OpinionSimulation.h"

int main()
{
    Graph graph{ "../../../testMatrix.txt" };
    OpinionSimulation simulation{ &graph };

    simulation.run();
}