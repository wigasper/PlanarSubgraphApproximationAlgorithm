#include "AdjacencyList.cpp"
#ifndef PlanarGraphHeuristic
#define PlanarGraphHeuristic

Graph algorithmA(Graph *graph);

std::vector<std::vector<int>> getComponents(Graph *graph);

std::list<int> nodeBFS(int startNode, Graph *graph);

std::list<std::pair<int, int>> edgeBFS(int startNode, Graph *graph);

#endif