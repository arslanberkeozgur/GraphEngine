#include "Input.h"

namespace Input
{
    void HandleInputs(sf::RenderWindow& window, InputState& inputState, sf::View& view, Graph& graph)
    {
        while (const auto event = window.pollEvent())
        {
            // These two are for closing the application.
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
                }
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::P)
                {
                    inputState.keyPressed_P = true;
                }
            }

            if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                if (keyReleased->scancode == sf::Keyboard::Scancode::P)
                {
                    inputState.keyPressed_P = false;
                }
            }

            // Some bookkeeping for important input actions.
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    // Select closest node once.
                    if (!inputState.mouseRightPressed)
                    {
                        inputState.selectedNodeIndexForInfo = graph.findClosestNode(window.mapPixelToCoords(mouseButtonPressed->position));
                    }
                    inputState.mouseLeftPressed = true;
                }
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    // Select closest node once.
                    if (!inputState.mouseRightPressed)
                    {
                        inputState.selectedNodeIndexForMoving = graph.findClosestNode(window.mapPixelToCoords(mouseButtonPressed->position));
                    }
                    inputState.mouseRightPressed = true;
                }
            }
            if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    inputState.mouseLeftPressed = false;
                }
                if (mouseButtonReleased->button == sf::Mouse::Button::Right)
                {
                    inputState.mouseRightPressed = false;
                    inputState.selectedNodeIndexForMoving = -1;
                }
            }
            // Update mouse movement.
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                if (inputState.firstFrame)
                {
                    inputState.lastMousePos = mouseMoved->position;
                    inputState.lastMouseWorldPos = window.mapPixelToCoords(mouseMoved->position);
                    inputState.firstFrame = false;
                }
                else
                {
                    inputState.deltas.deltaMouse = mouseMoved->position - inputState.lastMousePos;
                    sf::Vector2f currentMouseWorldPos = window.mapPixelToCoords(mouseMoved->position);
                    inputState.deltas.deltaMouseWorld = currentMouseWorldPos - inputState.lastMouseWorldPos;
                    inputState.lastMousePos = mouseMoved->position;
                    inputState.lastMouseWorldPos = currentMouseWorldPos;
                }
            }
            // Zooming
            if (const auto* mouseScroll = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (!inputState.firstFrame)
                {
                    view.zoom(1.0 - 0.05 * mouseScroll->delta);
                    sf::Vector2f potentialCenter = view.getCenter();
                    float halfX = view.getSize().x / 2;
                    float halfY = view.getSize().y / 2;

                    window.setView(view);
                    inputState.lastMouseWorldPos = window.mapPixelToCoords(inputState.lastMousePos);

                }
            }
        }
    }

    void ResetDeltas(InputState& inputState)
    {
        inputState.deltas = InputDeltas{};
    }
}