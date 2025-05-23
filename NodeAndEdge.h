#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>

struct Edge;

struct Node
{
	sf::Vector2f position; // This is for rendering only. (position in world)
	unsigned int index;

	std::vector<Edge*> inEdges;
	std::vector<Edge*> outEdges;

	// ---------- YOU CAN ADD PROPERTIES BELOW THIS LINE ----------

	// ---------- YOU CAN ADD PROPERTIES ABOVE THIS LINE ----------
};

struct Edge
{
	sf::Vector2f position; // This is for rendering only. (position in world)
	sf::Vector2f normal; // Rendering and text positioning. Normal vector of edge.

	Node* start;
	Node* end;
	unsigned int index;

	// ---------- YOU CAN ADD & CHANGE PROPERTIES BELOW THIS LINE ----------
	int weight;
	// ---------- YOU CAN ADD & CHANGE PROPERTIES ABOVE THIS LINE ----------
};