#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <vector>

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

// because the edges are sorted
// so we borrow that to use unordered_map to have access complexity is O1
auto buildAdjacencyList(const std::vector<std::pair<int,int>>& edges) -> std::unordered_map<int, std::vector<int>>
{
    std::unordered_map<int, std::vector<int>> adjList;
    for (const auto& edge : edges)
    {
        adjList[edge.first].push_back(edge.second);
        adjList[edge.second].push_back(edge.first);  // Since it's an undirected graph
    }

    return adjList;
}

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

// Time complexity: O(E + V)
//  for all vertices (on stack) and edges  
//
// Space O(V) to store visited node
void dfs()
{
    auto edges = edgeList;

    // Step 1: Build the adjacency list from the edges
    std::unordered_map<int, std::vector<int>> adjList {buildAdjacencyList(edges)};

    for (const auto& [e, as] : adjList)
    {
        std::cout << e << " : ";
        for (const auto a: as)
        {
            std::cout << a << " ";

        }
        std::cout <<"\n";
    }

    // Step 2: Perform DFS starting from node 1 (or any node)
    std::unordered_set<int> visited;
    std::cout << "DFS starting from node 1: ";

    auto node = 1;

    // Create a stack to hold the DFS nodes
    std::stack<int> s;
    s.push(node);

    while (!s.empty())
    {
        int current = s.top();
        s.pop();
        
        if (visited.find(current) == visited.end())
        {
            std::cout << current << " ";  // Process the current node
            visited.insert(current);      // Mark the node as visited
        }

        // Explore neighbors
        for (int neighbor : adjList.at(current))
        {
            if (visited.find(neighbor) == visited.end())
            {
                s.push(neighbor);  // Push unvisited neighbors onto the stack
            }
        }
    }
}

// the same as the stack solution
// recursive is simple way to leave the state for syscall and real stack
// this version is for a small graph, otherwise we get the stackoverflow due to too many recursion 
void dfsRecursive(int node, std::unordered_map<int, std::vector<int>>& adjList, std::unordered_set<int>& visited) {
    // Mark the current node as visited and process it
    visited.insert(node);
    std::cout << node << " ";

    // Explore all neighbors of the current node
    for (int neighbor : adjList[node]) {
        // If the neighbor hasn't been visited, visit it recursively
        if (visited.find(neighbor) == visited.end()) {
            dfsRecursive(neighbor, adjList, visited);
        }
    }
}

void bfs(int start)
{
    std::unordered_set<int> visited;

    // Step 1: Build the adjacency list from the edges
    std::unordered_map<int, std::vector<int>> adjList {buildAdjacencyList(edgeList)};

    // Step 2: the queue for BFS
    std::queue<int> q;

    // Step 3: Start from "start"
    visited.insert(start);
    q.push(start);

    while(!q.empty())
    {
        auto current = q.front();
        q.pop();

        // Step 4: process current node
        std::cout<< current <<" ";

        // Step 5: enqueue children of current node
        for (const auto& node : adjList.at(current))
        {
            if (visited.find(node) == visited.end())
            {
                visited.insert(node);
                q.push(node);
            }
        }
    }

}

void bfs_helper_for_countIsland(std::vector<std::vector<char>>& grid, int x, int y)
{
    // 4 directions to spread out
    const static std::vector directions = {-1, 0 , 1, 0, -1};
    std::queue<std::pair<int,int>> q;
    q.push({x,y});

    const auto noRow = grid.size();
    const auto noCol = grid.front().size();

    while(!q.empty())
    {
        auto [rowIndex, colIndex]  = q.front();
        q.pop();

        // 4 is for direction
        for (size_t i = 0; i < 4; i++)
        {
            auto newRowIndex = rowIndex + directions[i];
            auto newColIndex = colIndex + directions[i+1];

            // check if new cell is within the grid
            if ((newRowIndex >= 0 && newRowIndex < noRow) &&
                (newColIndex >= 0 && newColIndex < noCol) &&
                (grid[newRowIndex][newColIndex] == '1'))
            {
                grid[newRowIndex][newColIndex] = '0'; // visited cell
                q.push({newRowIndex, newColIndex}); // dequeue cell
            }
        }
    }
}

// count the connected component
int countIsland()
{
    std::vector<std::vector<char>> grid =
    {
        {'1', '1', '1', '1', '0'},
        {'1', '1', '0', '1', '0'},
        {'1', '1', '0', '0', '1'},
        {'0', '0', '0', '1', '0'}
    };

    int noRow = grid.size();
    int noCol = grid.front().size();

    int noIsland = 0;

    for (size_t r = 0; r < noRow; r++)
    {
        for (size_t c = 0; c < noCol; c++)
        {
            if (grid[r][c] == '1')
            {
                noIsland++;
                bfs_helper_for_countIsland(grid, r, c);
            }
        }
        
    }

    return noIsland;
}

int main()
{
    std::cout<<"count island "<<countIsland();
    return 0;
}
