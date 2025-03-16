#pragma once 

#include "Graph.h"

namespace Input
{
    struct InputDeltas
    {
        sf::Vector2i deltaMouse = { 0, 0 };
        sf::Vector2f deltaMouseWorld = { 0.0f, 0.0f };
    };

    struct InputState
    {
        int selectedNodeIndexForMoving = -1;
        int selectedNodeIndexForInfo = -1;
        bool firstFrame = true;
        bool mouseLeftPressed = false;
        bool mouseRightPressed = false;
        bool keyPressed_P = false;
        sf::Vector2i lastMousePos;
        sf::Vector2f lastMouseWorldPos;
        InputDeltas deltas;
    };

    void HandleInputs(sf::RenderWindow& window, InputState& inputState, sf::View& view, Graph& graph);
    void ResetDeltas(InputState& inputState);
}