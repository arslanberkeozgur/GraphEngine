#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>

struct Edge;

struct Node
{
	sf::Vector2f position; // This is for rendering only. (position in world)

	std::vector<Edge*> inEdges;
	std::vector<Edge*> outEdges;

	// ---------- YOU CAN ADD PROPERTIES BELOW THIS LINE ----------
	int persuation = 1;
	int gullability = 1;

	// ---------- YOU CAN ADD PROPERTIES ABOVE THIS LINE ----------
};

struct Edge
{
	sf::Vector2f position; // This is for rendering only. (position in world)
	sf::Vector2f normal; // Rendering and text positioning. Normal vector of edge.

	Node* start;
	Node* end;
	unsigned int index;

	// ---------- YOU CAN ADD PROPERTIES BELOW THIS LINE ----------
	int weight;

	// ---------- YOU CAN ADD PROPERTIES ABOVE THIS LINE ----------
};