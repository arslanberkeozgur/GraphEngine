# C++ Graph Engine

## Overview
This project implements a **Graph Engine** using **C++ and SFML** for visualization. It provides:
- A **Graph** class with nodes and edges stored via an adjacency matrix.
- A **Simulation** base class managing rendering, input, and a simulation loop.
- The ability to create custom simulations by inheriting from `Simulation`.

## Setup & Installation
### 1. Install SFML
1. Download SFML from [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php).
2. Extract the SFML folder to a known location.
3. Ensure you have the correct **x64 or x86** version matching your compiler.

### 2. Clone the Repository
```sh
$ git clone https://github.com/your-repo/graph-engine.git
$ cd graph-engine
```

### 3. Configure CMake for Visual Studio
1. Open **Visual Studio**.
2. Select **"Open a CMake Project"** and choose this folder.
3. Modify `CMakeLists.txt` if necessary:
```cmake
set(SFML_ROOT "C:/Path/To/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
```

### 4. Build the Project
- Configure CMake in **Release** or **Debug** mode.
- Compile using **Visual Studio’s Build menu**.

### 5. Copy SFML DLLs to Executable Folder
After building, copy required SFML `.dll` files (e.g., `sfml-graphics-2.dll`, `sfml-window-2.dll`, etc.) to the `build/Debug/` or `build/Release/` directory where the `.exe` is located.

## Running the Example Simulation
To run the example `OpinionSimulation`:
```cpp
#include "OpinionSimulation.h"
#include "GraphGeneration.h"

int main()
{
    std::vector<std::vector<int>> adjacencyMatrix = GraphGeneration::GenerateRandomGraph(200, 0.01, 4);
    Graph graph{ adjacencyMatrix };
    OpinionSimulation simulation{ &graph };
    simulation.run();
    return 0;
}
```
- This initializes a **random graph** with 200 nodes and a 1% edge probability.
- The `OpinionSimulation` class runs inside the `SFML` window, processing input and visualizing node interactions.

## User Input Controls
| Action | Effect |
|--------|--------|
| **Left-click & drag** | Pan the camera |
| **Right-click on a node** | Pick up and drag the node |
| **Escape (Esc) key** | Close the window |

## Understanding Nodes and Edges
### Node Structure
```cpp
struct Node
{
    sf::Vector2f position;
    unsigned int index;
    std::vector<Edge*> inEdges;
    std::vector<Edge*> outEdges;
};
```
Each node has:
- A `position` for rendering.
- A unique `index`.
- Lists of incoming (`inEdges`) and outgoing (`outEdges`) edges.

You can add additional fields to these structures for you custom simulations. In fact, this is a crucial step in defining your own simulation. 

### Edge Structure
```cpp
struct Edge
{
    Node* start;
    Node* end;
    int weight;
};
```
Each edge connects two nodes and has an integer `weight`.

## Creating Custom Simulations
To create your own simulation, **inherit from `Simulation`** and override key functions.

### Example: Custom Simulation Class
```cpp
#pragma once
#include "Simulation.h"
#include <random>

class MySimulation : public Simulation
{
public:
    using Simulation::Simulation;
    void onStart() override;
    void onStep() override;
    void injectInputHandling() override;
};
```
### Implementing the Methods
```cpp
void MySimulation::onStart()
{
    // Runs once at startup
    recordValue("ExampleMetric", 0.0);
}

void MySimulation::onStep()
{
    // Runs every frame when simulation is active
    recordValue("ExampleMetric", rand() % 100);
}

void MySimulation::injectInputHandling()
{
    if (m_inputState.keyPressed_P)
    {
        if (m_isPaused) unpause();
        else pause();
    }
}
```

## Logging Data & Exporting CSV
You can **log values over time** and export them to a CSV file for visualization in Python:
```cpp
recordValue("NodeCount", m_graph->getNumOfNodes());
exportTrackedDataToCSV("output.csv");
```
### Reading CSV in Python
```python
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("output.csv")
plt.plot(data["Index"], data["NodeCount"])
plt.show()
```

## Available Graph Functions (`m_graph`)
| Function | Description |
|----------|-------------|
| `getNode(size_t index)` | Returns a node reference |
| `getEdge(size_t index)` | Returns an edge reference |
| `getNumOfNodes()` | Returns total nodes count |
| `getNumOfEdges()` | Returns total edges count |
| `getMinimumDegree()` | Minimum node degree |
| `getAverageDegree()` | Average node degree |
| `getMaximumDegree()` | Maximum node degree |

## Extending Input Handling
Override `injectInputHandling()` in your subclass to add custom input:
```cpp
void MySimulation::injectInputHandling()
{
    if (m_inputState.keyPressed_Space)
    {
        std::cout << "Spacebar pressed!" << std::endl;
    }
}
```

## Conclusion
This **Graph Engine** provides a base for network visualization and dynamic simulation development. By subclassing `Simulation`, you can create **custom algorithms**, **graph interactions**, and **visualized computations** with minimal setup.

---
For contributions or issues, feel free to submit a PR or open an issue!

