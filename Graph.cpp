#include "Graph.h"

#include <iostream>
#include <fstream>

// NOTE: Throughout the code "vertex" means rendering vertices and "nodes" mean the graph nodes.

// TODO: For now, vertices are intrinsically squares with 6 vertices and this number is hard-coded everywhere. 
// Maybe change this later.

// Constructor with an adjacency matrix input.
Graph::Graph(const std::vector<std::vector<int>>& adjacencyMatrix)
	: m_areaSize{static_cast<float>(adjacencyMatrix.size())},
	  m_nodes{adjacencyMatrix.size()},
	  m_adjacencyMatrix{adjacencyMatrix},
	  m_nodeVertices{sf::PrimitiveType::Triangles, adjacencyMatrix.size() * 6}
{ 
	adjacencyMatrixToGeometry();
}

// Constructor with a path to an adjacency matrix.
Graph::Graph(const std::string& path)
{
	readAdjacencyMatrixFromFile(path);
	m_areaSize = static_cast<float>(m_adjacencyMatrix.size());
	m_nodes.resize(m_adjacencyMatrix.size());
	m_nodeVertices = sf::VertexArray(sf::PrimitiveType::Triangles, m_adjacencyMatrix.size() * 6);

	adjacencyMatrixToGeometry();
}


void Graph::adjacencyMatrixToGeometry()
{
	// For now seed for reproducibility.
	std::srand(0);
	// Number of vertices
	const unsigned int numOfNodes = static_cast<int>(m_adjacencyMatrix.size());

	if (numOfNodes == 0)
	{
		std::cerr << "WARNING::PASSED IN AN EMPTY ADJACENCY MATRIX" << std::endl;
		return;
	}

	// Process edges. First pass to determine the number of edges.
	unsigned int numOfEdges = 0;
	for (int i = 0; i < numOfNodes; ++i)
	{
		m_nodes[i].index = i;
		for (int j = i + 1; j < numOfNodes; ++j)
		{
			if (m_adjacencyMatrix[i][j] != 0)
			{
				numOfEdges++;
			}
			if (m_adjacencyMatrix[j][i] != 0)
			{
				numOfEdges++;
			}
		}
	}

	m_edges.resize(numOfEdges);
	m_edgeVertices = sf::VertexArray(sf::PrimitiveType::Triangles, numOfEdges * 9);

	// Second pass to set the edges.
	unsigned int currentEdge = 0;
	for (int i = 0; i < numOfNodes; ++i)
	{
		for (int j = i + 1; j < numOfNodes; ++j)
		{
			if (m_adjacencyMatrix[i][j] != 0)
			{
				Edge& edge = m_edges[currentEdge];

				edge.start = &m_nodes[i];
				edge.end = &m_nodes[j];
				edge.index = currentEdge;
				edge.weight = m_adjacencyMatrix[i][j];

				m_nodes[i].outEdges.push_back(&edge);
				m_nodes[j].inEdges.push_back(&edge);

				setEdgeColor(currentEdge, Settings::EDGE_COLOR, Settings::EDGE_ALPHA);

				currentEdge++;
			}
			if (m_adjacencyMatrix[j][i] != 0)
			{
				Edge& edge = m_edges[currentEdge];

				edge.start = &m_nodes[j];
				edge.end = &m_nodes[i];
				edge.index = currentEdge;
				edge.weight = m_adjacencyMatrix[j][i];

				m_nodes[i].inEdges.push_back(&edge);
				m_nodes[j].outEdges.push_back(&edge);

				setEdgeColor(currentEdge, Settings::EDGE_COLOR, Settings::EDGE_ALPHA);

				currentEdge++;
			}
		}
	}

	// Initialize the colors
	// First assign random positions to all vertices
	for (size_t i = 0; i < numOfNodes; ++i)
	{
		setNodeColor(i, Settings::NODE_COLOR);

		float x = randFloat() * m_areaSize - (m_areaSize / 2.0f);
		float y = randFloat() * m_areaSize - (m_areaSize / 2.0f);
		m_nodes[i].position = sf::Vector2f{ x, y };
	}

	// Iterative position determination.
	for (int iter = 0; iter < m_iterations; ++iter)
	{
		updateGeometry();
	}
}

void Graph::updateGeometry(int nodeHeld, sf::Vector2f inject)
{
	unsigned int numOfNodes = m_adjacencyMatrix.size();

	// Temporary array for force sums.
	std::vector<sf::Vector2f> forces{ numOfNodes, sf::Vector2f{0.0f, 0.0f} };

	// Compute forces to make nodes appear separate in space.
	for (int i = 0; i < numOfNodes; ++i)
	{
		for (int j = i + 1; j < numOfNodes; ++j)
		{
			sf::Vector2f dr = m_nodes[j].position - m_nodes[i].position;
			float dist = dr.length();

			// Avoid infinite forces
			if (dist < 1e-2f)
			{
				dist = 1e-2f;
			}

			// Calculate ~1/r force. F = -(k/r) * r_hat.
			// r_hat points from i to j.
			sf::Vector2f r_hat = dr / dist;
			sf::Vector2f repulsiveForce = (m_repulsion / dist) * r_hat;

			// Third law, equal and opposite.
			if (nodeHeld != i)
			{
				forces[i] -= repulsiveForce;
			}
			forces[j] += repulsiveForce;

			// Spring (attractive) forces.
			if (m_adjacencyMatrix[i][j] != 0 || m_adjacencyMatrix[j][i] != 0)
			{
				// Hook's force ~r. F = k * r * r_hat.
				sf::Vector2f attractiveForce = m_attraction * dist * r_hat;
				if (nodeHeld != i)
				{
					forces[i] += attractiveForce;
				}
				forces[j] -= attractiveForce;
			}
		}
	}

	// Update vertex positions of nodes for rendering.
	for (int i = 0; i < numOfNodes; ++i)
	{
		m_nodes[i].position += forces[i];
		if (nodeHeld == i)
		{
			m_nodes[i].position = inject;
		}
		setVertexPositionsOfNode(i);
	}

	// Update vertex positions of edges for rendering.
	for (int i = 0; i < m_edges.size(); ++i)
	{
		setVertexPositionsOfEdge(i);
	}
}

void Graph::setVertexPositionsOfNode(size_t i)
{
	sf::Vector2f position = m_nodes[i].position;
	m_nodeVertices[6 * i].position = { position.x, position.y };
	m_nodeVertices[6 * i + 1].position = { position.x + m_nodeSize, position.y };
	m_nodeVertices[6 * i + 2].position = { position.x, position.y + m_nodeSize };
	m_nodeVertices[6 * i + 3].position = { position.x + m_nodeSize, position.y + m_nodeSize };
	m_nodeVertices[6 * i + 4].position = { position.x + m_nodeSize, position.y };
	m_nodeVertices[6 * i + 5].position = { position.x, position.y + m_nodeSize };
}

void Graph::readAdjacencyMatrixFromFile(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "ERROR::COULD NOT OPEN FILE: " << path << std::endl;
		return;
	}

	// Read the first line to deduce the size
	std::string line;
	if (!std::getline(file, line))
	{
		std::cerr << "ERROR::FILE " << path << " APPEARS TO BE EMPTY." << std::endl;
		return;
	}

	std::istringstream iss(line);
	std::vector<int> firstRow;
	int temp;
	while (iss >> temp)
	{
		firstRow.push_back(temp);
	}

	const size_t N = firstRow.size();
	if (N == 0)
	{
		std::cerr << "ERROR:: FIRST ROW EMPTY, CANNOT DEDUCE MATRIX SIZE " << std::endl;
		return;
	}

	m_adjacencyMatrix.clear();
	m_adjacencyMatrix.resize(N, std::vector<int>(N, 0));

	for (size_t j = 0; j < N; ++j)
	{
		m_adjacencyMatrix[0][j] = firstRow[j];
	}

	for (size_t i = 1; i < N; ++i)
	{
		if (!std::getline(file, line))
		{
			std::cerr << "ERROR::FILE DOES NOT CONTAIN ENOUGH LINES TO FORM THE MATRIX" << std::endl;
			return;
		}

		std::istringstream issRow(line);
		for (size_t j = 0; j < N; ++j)
		{
			if (!(issRow >> m_adjacencyMatrix[i][j]))
			{
				std::cerr << "ERROR::LINE " << i << " DOES NOT CONTAIN ENOUGH INTEGERS" << std::endl;
				return;
			}
		}
	}

	file.close();
}

void Graph::setVertexPositionsOfEdge(size_t i, bool second)
{
	Node* start = m_edges[i].start;
	Node* end = m_edges[i].end;

	if (!start || !end)
	{
		std::cerr << "ERROR::START OR END NODES OF EDGE " << i << "NON-EXISTENT" << std::endl;
		return;
	}

	// Geometry:
	//           1,5   3
	//            B    D
	//     pos1  * ==== * pos2
	//            A    C
	//            0   2,4 

	// Offset to center so that edges are located between centers of nodes.
	sf::Vector2 centerOffset = { 0.5f * m_nodeSize, 0.5f * m_nodeSize };

	// For directed networks, we can have two edges between two nodes, so adjust offset.
	// Second edge lies below, first edge lies above.
	if (second)
	{
		centerOffset += {0.0f, 0.5f * centerOffset.y};
	}
	else
	{
		centerOffset -= {0.0f, 0.5f * centerOffset.y};
	}

	sf::Vector2f pos1 = start->position + centerOffset;
	sf::Vector2f pos2 = end->position + centerOffset;
	sf::Vector2f dr = pos2 - pos1;

	m_edges[i].position = pos1 + 0.5f * dr;

	// Avoid small distances, division by zero errors.
	float dist = dr.length();
	if (dist < 1e-2f)
	{
		dist = 1e-2f;
	}

	sf::Vector2f dr_hat = dr / dist;
	// Perpendicular to dr_hat.
	sf::Vector2f perp(-dr_hat.y, dr_hat.x);

	m_edges[i].normal = perp;

	// Calculate the four corners
	// We move halfThickness "left" and "right" along the perpendicular
	sf::Vector2f offset = perp * m_edgeThickness / 2.0f;

	sf::Vector2f cornerA = pos1 - offset; // bottom-left  
	sf::Vector2f cornerB = pos1 + offset; // top-left
	sf::Vector2f cornerC = pos2 - offset; // bottom-right
	sf::Vector2f cornerD = pos2 + offset; // top-right

	m_edgeVertices[9 * i].position     = cornerA;
	m_edgeVertices[9 * i + 1].position = cornerB;
	m_edgeVertices[9 * i + 2].position = cornerC;
	m_edgeVertices[9 * i + 3].position = cornerD;
	m_edgeVertices[9 * i + 4].position = cornerC;
	m_edgeVertices[9 * i + 5].position = cornerB;

	// Arrow
	m_edgeVertices[9 * i + 6].position = pos2 - 0.5f * dr;
	m_edgeVertices[9 * i + 7].position = cornerA + 0.45f * dr;
	m_edgeVertices[9 * i + 8].position = cornerB + 0.45f * dr;
	m_edgeVertices[9 * i + 6].color = sf::Color::Black;
	m_edgeVertices[9 * i + 7].color = sf::Color::Black;
	m_edgeVertices[9 * i + 8].color = sf::Color::Black;

	// If this is not the second edge between these vertices, look for a second edge.
	if (!second)
	{
		for (size_t i = 0; i < end->outEdges.size(); ++i)
		{
			if (end->outEdges[i]->end == start)
			{
				setVertexPositionsOfEdge(end->outEdges[i]->index, true);
			}
		}
	}
}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = nullptr;
	target.draw(m_edgeVertices, states);
	target.draw(m_nodeVertices, states);
}

// Helpers
int Graph::findClosestNode(sf::Vector2f position, float tolerance) const
{
	if (m_nodes.empty())
	{
		std::cerr << "WARNING:: NO NODES EXIST IN THE GRAPH." << std::endl;
		return -1;
	}

	size_t closestIndex = 0;
	float minDistanceSquared = std::numeric_limits<float>::max();

	for (size_t i = 0; i < m_nodes.size(); ++i)
	{
		sf::Vector2f diff = m_nodes[i].position - position;
		float distSquared = diff.lengthSquared();

		if (distSquared < minDistanceSquared)
		{
			minDistanceSquared = distSquared;
			closestIndex = i;
		}
	}

	if (minDistanceSquared > tolerance)
		closestIndex = -1;

	return closestIndex;
}

void Graph::setNodeColor(size_t index, sf::Color color, float alpha = 1.0f)
{
	m_nodeVertices[6 * index].color = color;
	m_nodeVertices[6 * index + 1].color = color;
	m_nodeVertices[6 * index + 2].color = color;
	m_nodeVertices[6 * index + 3].color = color;
	m_nodeVertices[6 * index + 4].color = color;
	m_nodeVertices[6 * index + 5].color = color;

	m_nodeVertices[6 * index].color.a = int(alpha * 255.0f);
	m_nodeVertices[6 * index + 1].color.a = int(alpha * 255.0f);
	m_nodeVertices[6 * index + 2].color.a = int(alpha * 255.0f);
	m_nodeVertices[6 * index + 3].color.a = int(alpha * 255.0f);
	m_nodeVertices[6 * index + 4].color.a = int(alpha * 255.0f);
	m_nodeVertices[6 * index + 5].color.a = int(alpha * 255.0f);

}

void Graph::setEdgeColor(size_t index, sf::Color color, float alpha = 1.0f)
{
	m_edgeVertices[9 * index].color = color;
	m_edgeVertices[9 * index + 1].color = color;
	m_edgeVertices[9 * index + 2].color = color;
	m_edgeVertices[9 * index + 3].color = color;
	m_edgeVertices[9 * index + 4].color = color;
	m_edgeVertices[9 * index + 5].color = color;

	m_edgeVertices[9 * index].color.a = int(alpha * 255.0f);
	m_edgeVertices[9 * index + 1].color.a = int(alpha * 255.0f);
	m_edgeVertices[9 * index + 2].color.a = int(alpha * 255.0f);
	m_edgeVertices[9 * index + 3].color.a = int(alpha * 255.0f);
	m_edgeVertices[9 * index + 4].color.a = int(alpha * 255.0f);
	m_edgeVertices[9 * index + 5].color.a = int(alpha * 255.0f);
}