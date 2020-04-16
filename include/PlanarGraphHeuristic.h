// This work is heavily comprised of Nathan Cornelius's original code from
// github.com/ndcornelius/PlanarSubgraphApproximationAlgorithm

#include "../src/AdjacencyList.cpp"
#ifndef PlanarGraphHeuristic
#define PlanarGraphHeuristic

// TODO: Should these really be in this header?
//
Graph algorithmA(Graph *graph);

std::vector<std::vector<int>> getComponents(Graph *graph);

std::list<int> nodeBFS(int startNode, Graph *graph);

std::list<std::pair<int, int>> edgeBFS(int startNode, Graph *graph);

Graph multipleComponentAlgorithmA(Graph *graph);

#endif
