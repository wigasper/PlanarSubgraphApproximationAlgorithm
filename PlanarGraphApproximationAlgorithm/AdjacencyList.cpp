
#include <iostream>
#include <vector>
#include <list>
#include <map>

typedef int node;
typedef std::pair<node, node> edge;
typedef std::list<edge> edge_list;

class Graph {
private:
	//std::vector<std::list<int>> adjacencyListArray;
	std::map<node, std::list<node>> adjacencyListArray;

public:
	//Graph(int n) {
		//adjacencyListArray = std::vector<std::list<int>>(n);
	//}

	std::list<node> getNodes() {
		std::list<node> nodeList;
		for (std::map<node, std::list<node>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			nodeList.push_back((*iter).first);
		}

		return nodeList;
	}

	void addNode(node label) {
		adjacencyListArray[label];
	}

	void addEdge(int a, int b) {

		/*if ((size_t)a + 1 > adjacencyListArray.size() || (size_t)b + 1 > adjacencyListArray.size()){
			adjacencyListArray.resize((a > b ? a + 1 : b + 1));
		}

		adjacencyListArray[a].push_front(b);
		adjacencyListArray[b].push_front(a);
		*/
		adjacencyListArray[a].push_front(b);
		adjacencyListArray[b].push_front(a);

	}

	void addEdge(std::pair<int, int> edge) {

		int a = edge.first;
		int b = edge.second;

		/*if ((size_t)a + 1 > adjacencyListArray.size() || (size_t)b + 1 > adjacencyListArray.size()){
			adjacencyListArray.resize((a > b ? a + 1 : b + 1));
		}*/

		adjacencyListArray[a].push_front(b);
		adjacencyListArray[b].push_front(a);
	}

	std::list<int>* getList(int node) {
		return &adjacencyListArray.at(node);
	}

	int getNumNodes () {
		return adjacencyListArray.size();
	}

	int getNumEdges() {
		trim();
		int numEdges = 0;
		/*for (std::vector<std::list<int>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			numEdges += (*iter).size();
		}*/

		for (std::map<node, std::list<node>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			numEdges += (*iter).second.size();
		}

		return numEdges / 2;
	}

	//bool removeEdge(int a, int b) {
		//TODO
	//}

	void trim() {

		sort();
		/*for (std::vector<std::list<int>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			(*iter).unique(); 
		}*/

		for (std::map<node, std::list<node>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			(*iter).second.unique();
		}
	}

	void sort() {
		/*for (std::vector<std::list<int>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			(*iter).sort();
		}*/

		for (std::map<node, std::list<node>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			(*iter).second.sort();
		}
	}


	edge_list toEdgeList() {
		return toEdgeList(false);
	}

	edge_list toEdgeList(bool sortt) {

		if (sortt) {
			sort();
		}

		std::list<std::pair<int, int>> edge_list;

		for (std::map<node, std::list<node>>::iterator iter1 = adjacencyListArray.begin(); iter1 != adjacencyListArray.end(); iter1++) {
			std::list<node> adj = (*iter1).second;
			for (std::list<int>::iterator iter2 = adj.begin(); iter2 != adj.end(); iter2++) {
				edge_list.push_back(std::pair<int, int>((*iter1).first, *iter2));
			}
		}

		/*for (size_t i = 0; i < adjacencyListArray.size(); i++) {
			std::list<int> adj = adjacencyListArray[i];
			for (std::list<int>::iterator iter = adj.begin(); iter != adj.end(); iter++) {
				edge_list.push_back(std::pair<int, int>(i, *iter));
			}
		}*/

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

	int getNumNodesWithEdges(){

		int num = 0;

		/*for (std::vector<std::list<int>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			if ((*iter).size() != 0) {
				num++;
			}
		}*/

		for (std::map<node, std::list<node>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			if ((*iter).second.size() != 0) {
				num++;
			}
		}

		return num;
	}

	node first() {
		return (*adjacencyListArray.begin()).first;
	}

	/*void eliminateDisconnectedNodes() {

		std::vector<std::list<int>> oldAdjacencyListArray = adjacencyListArray;
		adjacencyListArray = std::vector<std::list<int>>(0);

		for (std::vector<std::list<int>>::iterator iter = adjacencyListArray.begin(); iter != adjacencyListArray.end(); iter++) {
			if ((*iter).size() != 0) {
				adjacencyListArray.push_back(*iter);
			}
		}
	}*/

};