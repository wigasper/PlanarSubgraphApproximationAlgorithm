
#include <iostream>
#include <vector>
#include <list>

typedef int node;
typedef std::pair<node, node> edge;
typedef std::list<edge> edge_list;

class Graph {
private:
	std::vector<std::list<int>> adjacencyListArray;

public:
	Graph(int n) {
		adjacencyListArray = std::vector<std::list<int>>(n);
	}

	void addEdge(int a, int b) {

		if ( a + 1 > adjacencyListArray.size() || b + 1 > adjacencyListArray.size()){
			adjacencyListArray.resize((a > b ? a + 1 : b + 1));
		}

		adjacencyListArray[a].push_front(b);
		adjacencyListArray[b].push_front(a);
	}

	void addEdge(std::pair<int, int> edge) {

		int a = edge.first;
		int b = edge.second;

		if (a + 1 > adjacencyListArray.size() || b + 1 > adjacencyListArray.size()){
			adjacencyListArray.resize((a > b ? a + 1 : b + 1));
		}

		adjacencyListArray[a].push_front(b);
		adjacencyListArray[b].push_front(a);
	}

	std::list<int>* getList(int node) {
		return &adjacencyListArray[node];
	}

	int nodeCount () {
		return adjacencyListArray.size();
	}

	bool removeEdge(int a, int b) {
		//TODO
	}

	edge_list toEdgeList() {
		std::list<std::pair<int, int>> edge_list;

		for (size_t i = 0; i < adjacencyListArray.size(); i++) {
			std::list<int> adj = adjacencyListArray[i];
			for (std::list<int>::iterator iter = adj.begin(); iter != adj.end(); iter++) {
				edge_list.push_back(std::pair<int, int>(i, *iter));
			}
		}

		return edge_list;
	}

	void addComponent(Graph *graph){
		int nodes = adjacencyListArray.size();
		
		std::list<std::pair<int, int>> edge_list = (*graph).toEdgeList();

		for (std::list<edge>::iterator edge = edge_list.begin(); edge != edge_list.end(); edge++) {
			addEdge((*edge).first + nodes, (*edge).second + nodes);
		}
	}

	void printGraph () {

		for (size_t i = 0; i < adjacencyListArray.size(); i++)  {
			std::cout << "\n" << i << ":";
			for (std::list<int>::iterator iter = adjacencyListArray[i].begin(); iter != adjacencyListArray[i].end(); iter++) {
				std::cout << " " << *iter;
			}
		}
	}

};