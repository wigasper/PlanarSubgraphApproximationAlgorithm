// This work is heavily comprised of Nathan Cornelius's original code from
// github.com/ndcornelius/PlanarSubgraphApproximationAlgorithm

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

typedef int node;
typedef std::pair<node, node> edge;
typedef std::list<edge> edge_list;

class Graph {
    //  private:
    //    std::map<node, std::list<node>> adjacencyListArray;

  public:
    std::map<node, std::list<node>> adjacencyListArray;
    bool is_poison_pill = false;

    std::list<node> getNodes() {
	std::list<node> nodeList;
	for (const auto &[key_node, adjacents] : adjacencyListArray) {
	    nodeList.push_back(key_node);
	}

	return nodeList;
    }

    void addNode(node label) { adjacencyListArray[label]; }

    void addEdge(node node_0, node node_1) {
	adjacencyListArray[node_0].push_front(node_1);
	adjacencyListArray[node_1].push_front(node_0);
    }

    void addEdge(std::pair<node, node> edge) {
	node node_0 = edge.first;
	node node_1 = edge.second;

	adjacencyListArray[node_0].push_front(node_1);
	adjacencyListArray[node_1].push_front(node_0);
    }

    std::list<node> *getList(node node) { return &adjacencyListArray.at(node); }

    int getNumNodes() { return adjacencyListArray.size(); }

    int getNumEdges() {
	trim();

	int numEdges = 0;

	for (const auto &[key_node, adjacents] : adjacencyListArray) {
	    numEdges += adjacents.size();
	}

	return numEdges / 2;
    }

    void trim() {
	sort();
	for (auto &[key_node, adjacents] : adjacencyListArray) {
	    adjacents.unique();
	}
    }

    void sort() {
	for (auto &[key_node, adjacents] : adjacencyListArray) {
	    adjacents.sort();
	}
    }

    edge_list toEdgeList() { return toEdgeList(false); }

    edge_list toEdgeList(bool sortt) {

	if (sortt) {
	    sort();
	}

	std::list<std::pair<node, node>> edges_out;

	for (const auto &[key_node, adjacents] : adjacencyListArray) {
	    std::list<node> adjacent_nodes = adjacents;
	    for (node adj_node : adjacent_nodes) {
		edges_out.push_back(std::pair<node, node>(key_node, adj_node));
	    }
	}

	return edges_out;
    }

    int getNumNodesWithEdges() {

	int num = 0;

	for (const auto &[key_node, adjacents] : adjacencyListArray) {
	    if (adjacents.size() != 0) {
		num++;
	    }
	}

	return num;
    }

    node first() { return (*adjacencyListArray.begin()).first; }
};
