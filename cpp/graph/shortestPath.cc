#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>

namespace GraphAlgorithms {

    // Struct to represent an edge with destination node and weight
    struct Edge {
        int destinationNode;
        int weight;

        // Constructor to initialize an edge
        Edge(int dest, int wt) : destinationNode(dest), weight(wt) {}
    };

    // Struct to represent a node and its distance in the priority queue
    struct NodeDistance {
        int node;
        int distance;

        // Constructor to initialize the node and distance
        NodeDistance(int n, int d) : node(n), distance(d) {}

        // Overload the comparison operator for priority queue (min-heap based on distance)
        bool operator>(const NodeDistance& other) const {
            return distance > other.distance;
        }
    };

    class Graph {
    public:
        // Adjacency list to store the graph as a map of nodes and their outgoing edges
        std::unordered_map<int, std::vector<Edge>> adjacencyList;

        // Add an edge to the graph (from u to v with weight w)
        void addEdge(int fromNode, int toNode, int weight) {
            adjacencyList[fromNode].push_back(Edge(toNode, weight));
            adjacencyList[toNode].push_back(Edge(fromNode, weight)); // For undirected graph
        }

        // Dijkstra's algorithm to find the shortest path from start to end
        std::vector<int> findShortestPath(int startNode, int endNode) {
            // Step 1: Initialize distances and priority queue
            std::unordered_map<int, int> distance;
            std::unordered_map<int, int> parentNode;
            std::priority_queue<NodeDistance, std::vector<NodeDistance>, std::greater<NodeDistance>> priorityQueue;

            // Initialize all distances to infinity
            for (const auto& node : adjacencyList) {
                distance[node.first] = INT_MAX;
            }

            // Set the distance to the start node as 0
            distance[startNode] = 0;
            priorityQueue.push(NodeDistance(startNode, 0));  // {node, distance}

            // Step 2: Dijkstra's algorithm (relaxation)
            while (!priorityQueue.empty()) {
                int currentNode = priorityQueue.top().node;
                priorityQueue.pop();

                // If we reached the destination node, stop the search
                if (currentNode == endNode) break;

                // Explore the neighbors of the current node
                for (const Edge& neighbor : adjacencyList[currentNode]) {
                    int neighborNode = neighbor.destinationNode;
                    int edgeWeight = neighbor.weight;

                    // Relaxation step: if a shorter path to the neighbor is found
                    if (distance[currentNode] + edgeWeight < distance[neighborNode]) {
                        distance[neighborNode] = distance[currentNode] + edgeWeight;
                        parentNode[neighborNode] = currentNode;
                        priorityQueue.push(NodeDistance(neighborNode, distance[neighborNode]));
                    }
                }
            }

            // Step 3: Reconstruct the path from start to end
            std::vector<int> path;
            if (distance[endNode] == INT_MAX) {
                return {}; // No path found
            }

            for (int currentNode = endNode; currentNode != startNode; currentNode = parentNode[currentNode]) {
                path.push_back(currentNode);
            }
            path.push_back(startNode);
            std::reverse(path.begin(), path.end());  // Reverse the path to get the correct order

            return path;
        }
    };

} // End of GraphAlgorithms namespace

int main() {
    GraphAlgorithms::Graph graph;

    // Add edges to the graph (from u to v with weight w)
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 1);
    graph.addEdge(2, 1, 2);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 8);
    graph.addEdge(3, 4, 3);
    graph.addEdge(4, 5, 1);

    int startNode = 0;
    int endNode = 5;

    std::vector<int> path = graph.findShortestPath(startNode, endNode);

    if (!path.empty()) {
        std::cout << "Shortest path from " << startNode << " to " << endNode << ": ";
        for (int node : path) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No path found!" << std::endl;
    }

    return 0;
}
