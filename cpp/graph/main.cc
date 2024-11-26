#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <cstdint>

// basis
// 
// G = <V, E>
//
// G: Graph
// V: Vertex
// E: Edge
//
// adjacency_matrix -> O(V * V)
// edge list        -> O(V * E)
// adjacency list   -> O(V + E)

constexpr uint64_t maxSize = 5;

const static std::vector<std::pair<int,int>> edgeList
{
    {1, 2},
    {1, 3},
    {1, 4},
    {2, 3},
    {2, 4},
    {2, 5},
    {3, 4},
    {3, 5},
    {4, 5},
};

const static std::unordered_map<int, std::vector<int>> adjacencyList
{
    {1, {2, 3, 4}   },
    {2, {3, 4, 5}   },
    {3, {4, 5}      },
    {4, {5}         },
};

void undirected_graph_to_adjacency_matrix()
{
    auto inputData = edgeList;

    std::transform(inputData.begin(), inputData.end(), inputData.begin(), [](auto& data)
    {
        data.first -= 1;
        data.second -= 1;
        return data;
    });

    std::vector<std::vector<int>> matrix(maxSize, std::vector<int>(maxSize, 0));
    for(const auto [x, y]: inputData)
    {
        // undirected_grap
        // Adjacency matrix
        matrix.at(x).at(y) = 1;
        matrix.at(y).at(x) = 1;
    }

    for (const auto row: matrix)
    {
        for (const auto it : row)
        {
            std::cout<<it<<" ";
        }
        std::cout<<"\n";
    }

}

// depth first search
void dfs(int u)
{
    static auto inputData = adjacencyList;
    static std::vector<bool> visited (maxSize, false);
    visited.at(u - 1) = true;
    for (const auto [v, es]: inputData)
    {
        for (const auto e : es)
        {
            std::cout<< e << " ";
            if (!visited.at(e - 1))
            {
                dfs(e);
            }
        }
    }

//     #include <iostream>
// #include <vector>
// #include <unordered_map>
// #include <unordered_set>
// #include <stack>

// void dfs(int node, const std::unordered_map<int, std::vector<int>>& adjList, std::unordered_set<int>& visited) {
//     // Create a stack to hold the DFS nodes
//     std::stack<int> s;
//     s.push(node);
    
//     while (!s.empty()) {
//         int current = s.top();
//         s.pop();
        
//         if (visited.find(current) == visited.end()) {
//             std::cout << current << " ";  // Process the current node
//             visited.insert(current);      // Mark the node as visited
//         }

//         // Explore neighbors
//         for (int neighbor : adjList.at(current)) {
//             if (visited.find(neighbor) == visited.end()) {
//                 s.push(neighbor);  // Push unvisited neighbors onto the stack
//             }
//         }
//     }
// }

// int main() {
//     std::vector<std::pair<int, int>> edges = {
//         {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4},
//         {2, 5}, {3, 4}, {3, 5}, {4, 5}
//     };

//     // Step 1: Build the adjacency list from the edges
//     std::unordered_map<int, std::vector<int>> adjList;
//     for (const auto& edge : edges) {
//         adjList[edge.first].push_back(edge.second);
//         adjList[edge.second].push_back(edge.first);  // Since it's an undirected graph
//     }

//     // Step 2: Perform DFS starting from node 1 (or any node)
//     std::unordered_set<int> visited;
//     std::cout << "DFS starting from node 1: ";
//     dfs(1, adjList, visited);

//     std::cout << std::endl;

//     return 0;
// }

}

int main()
{
    dfs(1);
    return 0;
}
