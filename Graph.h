#pragma once

#include "Settings.h"
#include "NodeAndEdge.h"
#include <vector>
#include <random>
#include <string>

// IMPORTANT: FOR NOW, ONCE YOU GET A NODE FROM THE GRAPH, DO NOT MESS WITH ITS CONNECTIONS.
// THE GRAPH IS NOT CODED TO ADAPTIVELY CHANGE ITS EDGES AND SO ON.
// CONSIDER THE GRAPH AS IMMUTABLE AND ONLY IMPLEMENT READ-ONLY ALGORITHMS.

class Graph : public sf::Drawable, public sf::Transformable
{
public:
	explicit Graph(const std::vector<std::vector<int>>& adjacencyMatrix);
	explicit Graph(const std::string& path);

	std::vector<std::vector<int>>& getAdjacencyMatrix()
	{
		return m_adjacencyMatrix;
	}
	Node& getNode(size_t index)
	{
		return m_nodes[index];
	}
	Edge& getEdge(size_t index)
	{
		return m_edges[index];
	}

	void updateGeometry(int nodeHeld = -1, sf::Vector2f inject = { 0, 0 });
	int findClosestNode(sf::Vector2f position, float tolerance = 10.0f) const;

	void setNodeColor(size_t index, sf::Color color, float alpha = 1.0f);
	void setEdgeColor(size_t index, sf::Color color, float alpha = 1.0f);

	int getNumOfEdges() const { return m_edges.size(); }
	int getNumOfNodes() const { return m_nodes.size();}
	unsigned int getMinimumDegree() const { return m_minimumDegree; }
	unsigned int getMinimumInDegree() const { return m_minimumInDegree; }
	unsigned int getMinimumOutDegree() const { return m_minimumOutDegree; }
	float getAverageDegree() const { return m_averageDegree; }
	float getAverageInDegree() const { return m_averageInDegree; }
	float getAverageOutDegree() const { return m_averageOutDegree; }
	unsigned int getMaximumDegree() const { return m_maximumDegree; }
	unsigned int getMaximumInDegree() const { return m_maximumInDegree; }
	unsigned int getMaximumOutDegree() const { return m_maximumOutDegree; }

private:
	std::vector<Node> m_nodes;
	std::vector<Edge> m_edges;
	std::vector<std::vector<int>> m_adjacencyMatrix;

	// Important parameters
	unsigned int m_minimumDegree;
	unsigned int m_minimumInDegree;
	unsigned int m_minimumOutDegree;
	unsigned int m_maximumDegree;
	unsigned int m_maximumInDegree;
	unsigned int m_maximumOutDegree;
	float m_averageDegree;
	float m_averageInDegree;
	float m_averageOutDegree;
	
	// Visual 
	float m_nodeSize = Settings::NODE_SIZE;
	float m_edgeThickness = Settings::EDGE_THICKNESS;
	float m_areaSize;
	float m_attraction = Settings::ATTRACTION_FORCE;
	float m_repulsion = Settings::REPULSION_FORCE;
	int m_iterations = Settings::NUMBER_OF_PHYSICS_ITERATIONS;

	// Rendering
	sf::VertexArray m_nodeVertices;
	sf::VertexArray m_edgeVertices;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setVertexPositionsOfNode(size_t i);
	void setVertexPositionsOfEdge(size_t i, bool second = false);
	void readAdjacencyMatrixFromFile(const std::string& path);
	void adjacencyMatrixToGeometry();

	// Helpers.
	float randFloat()
	{
		return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	}
	void calculateImportantParameters();
};