# Graph Engine

This is a minimalistic C++ graph visualization and simulation engine built using SFML. The engine is designed to generate, display, and analyze graphs while allowing basic interactions. The focus is on simplicity, direct access to data structures, and intuitive visual representation.

## Manual Usage

The engine provides interactive controls for manipulating and inspecting graph structures. The following inputs are available:

### Mouse Controls

- **Left Click & Drag:** Move a node.
- **Right Click & Drag:** Pan the view.
- **Scroll Wheel:** Zoom in and out.
- **Hover Over Node and Click:** Display node information.

### Keyboard Controls

- **P:** Pause/unpause simulation.
- **ESC:** Exit the application.

These controls allow users to explore graphs dynamically, observing their structures and properties in real time.

## Navigating the Codebase

The engine is designed to be easily extendable. To create and run simulations, follow these guidelines:

- **Modify `GraphEngine.cpp`** only for initialization and basic setup.
- **Inherit from `Simulation`** to define custom simulations by overriding specific methods.

### What You Have Access To in `Simulation`

When inheriting from `Simulation`, you have access to various components that allow interaction with the graph and control over the simulation:

- **`m_graph` (Graph*)**: Access the graph structure and its nodes/edges.
  - `m_graph->getNumOfNodes()`: Get the number of nodes.
  - `m_graph->getNumOfEdges()`: Get the number of edges.
  - `m_graph->getNode(size_t index)`: Access a specific node.
  - `m_graph->getEdge(size_t index)`: Access a specific edge.
  - `m_graph->setNodeColor(size_t index, sf::Color color)`: Change node color.
  - `m_graph->setEdgeColor(size_t index, sf::Color color)`: Change edge color.

- **Simulation State Variables**:
  - `m_isRunning`: Check if the simulation is running.
  - `m_isPaused`: Check if the simulation is paused.
  - `m_currentTimeStep`: Track the current timestep of the simulation.
  
- **Input Handling**:
  - `m_inputState`: Capture user inputs like key presses and mouse movements.
  - `injectInputHandling()`: Override to define custom input behavior.

### Example: Creating a Custom Simulation

To create a custom simulation, inherit from `Simulation` and implement the necessary methods. Below is an example implementation of an opinion dynamics simulation:

#### `OpinionSimulation.h`
```cpp
#pragma once

#include "Simulation.h"
#include <random>

class OpinionSimulation : public Simulation
{
public:
    using Simulation::Simulation;

    void onStart() override;
    void onStep() override;
    void injectInputHandling() override;

private:
    void injectInfoTextInitialization(std::stringstream& ss) override;
    void injectInfoTextUpdate(int nodeIndex, std::stringstream& ss) override;

    std::mt19937 rngEngine;
    std::uniform_int_distribution<> interactionDistribution{ 0,100 };

    std::vector<Edge*> flashingEdges;
    void flashEdges();
    void unflashEdges();
};
```

#### `OpinionSimulation.cpp`
```cpp
#include "OpinionSimulation.h"
#include <iostream>

void OpinionSimulation::onStart()
{
    flashingEdges.reserve(100);
    std::random_device rd;
    rngEngine.seed(rd());

    std::uniform_int_distribution<> intDist(1, 10);
    std::uniform_int_distribution<> broadcastFreqDist(20, 100);

    for (size_t i = 0; i < m_graph->getNumOfNodes(); ++i)
    {
        Node& node = m_graph->getNode(i);
        node.gullability = intDist(rngEngine);
        node.persuasion = intDist(rngEngine);
        node.opinionStrength = intDist(rngEngine);
        node.broadcastFrequency = broadcastFreqDist(rngEngine);

        int opinionDeterminer = intDist(rngEngine);
        if (opinionDeterminer > 5)
        {
            node.opinion = false;
            m_graph->setNodeColor(i, sf::Color::Blue);
        }
        if (node.opinion)
        {
            m_graph->setNodeColor(i, sf::Color::Red);
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
            for (Edge* connection : node.outEdges)
            {
                Node* endNode = connection->end;
                int successDeterminer = interactionDistribution(rngEngine);
                int interactionStrength = node.persuasion * endNode->gullability * connection->weight;

                if (successDeterminer < interactionStrength)
                {
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
                            m_graph->setNodeColor(endNode->index, endNode->opinion ? sf::Color::Red : sf::Color::Blue);
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
    for (Edge* edge : flashingEdges)
    {
        m_graph->setEdgeColor(edge->index, sf::Color::Green);
    }
}

void OpinionSimulation::unflashEdges()
{
    for (Edge* edge : flashingEdges)
    {
        m_graph->setEdgeColor(edge->index, sf::Color::White);
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
```

