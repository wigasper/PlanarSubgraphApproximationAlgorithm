#include "PlanarGraphHeuristic.h"
#include "AdjacencyList.h"
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <ctime>

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/boyer_myrvold_planar_test.hpp"

bool isPlanar(std::list<std::pair<int, int>> edge_list, int numNodes) {

	typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> boostGraph;
	
	boostGraph g(numNodes);

	for (std::list<std::pair<int, int>>::iterator iter = edge_list.begin(); iter != edge_list.end(); iter++) {
		boost::add_edge((*iter).first, (*iter).second, g);
	}

	return boyer_myrvold_planarity_test(g);

}

std::list<std::tuple<int, int, double>> loadFromFile(std::string file_path) {

	std::list<std::tuple<int, int, double>> edge_list;

	int a, b;
	double c;
	std::string line;
	std::ifstream file(file_path);

	if (file.good()){
		while (file >> a >> b >> c){
			edge_list.push_back(std::make_tuple(a, b, c));
		}
		file.close();
	}

	return edge_list;
}

void writeToFile(std::list<std::tuple<int, int, double>> edge_list, bool isPlanar, bool isPlanar2, int initialEdges, long seconds ) {

	int a, b;
	double c;
	long hours, minutes;

	minutes = seconds / 60;
	hours = minutes / 60;

	int resultEdges = edge_list.size();
	float percentRetained = (float)resultEdges / (float)initialEdges * 100;
	std::string planar = isPlanar ? "Yes" : "No";
	std::string planar2 = isPlanar2 ? "Yes" : "No";


	std::ofstream file("C:\\Users\\Nathan\\Desktop\\out.txt");

	file << "Original Planar: " << planar << "\n";
	file << "Result Planar: " << planar << "\n";
	file << "Execution time: " << hours << "h " << minutes % 60 << "m " << seconds % 60 << "s\n";
	file << "Initial number of edges: " << initialEdges << "\n";
	file << "Result number of edges: " << resultEdges << "\n";
	file << "Percent of edges retained: " << percentRetained << "%\n\n";

	for (std::list<std::tuple<int, int, double>>::iterator iter = edge_list.begin(); iter != edge_list.end(); iter++) {
		a = std::get<0>(*iter);
		b = std::get<1>(*iter);
		c = std::get<2>(*iter);
		file << a << " " << b << " " << c << "\n";
	}

	file.close();

}

int main(){

	std::list<std::tuple<int, int, double>> edge_list;
	edge_list = loadFromFile("C:\\Users\\Nathan\\Desktop\\dolphin.txt");

	std::list<std::pair<int, int>> unweighted_edge_list;

	Graph test_graph = Graph(0);

	for (std::list<std::tuple<int, int, double>>::iterator iter = edge_list.begin(); iter != edge_list.end(); iter++) {

		test_graph.addEdge(std::get<0>(*iter), std::get<1>(*iter));
		unweighted_edge_list.push_back(std::pair<int, int>(std::get<0>(*iter), std::get<1>(*iter)));

	}

	std::clock_t begin = clock();

	Graph p = algorithmA(test_graph);

	clock_t end = clock();

	long seconds;

	seconds = long(end - begin) / CLOCKS_PER_SEC;

	std::list<std::pair<int, int>> result_edge_list;
	std::list<std::tuple<int, int, double>> result_weighted_edge_list;
	result_edge_list = p.toEdgeList();

	for (std::list<std::tuple<int, int, double>>::iterator iter2 = edge_list.begin(); iter2 != edge_list.end(); iter2++) {
		if (std::find(result_edge_list.begin(), result_edge_list.end(), std::pair<int, int>(std::get<0>(*iter2), std::get<1>(*iter2))) != result_edge_list.end()) {
			result_weighted_edge_list.push_back(*iter2);
		}
	}

	writeToFile(result_weighted_edge_list, isPlanar(result_edge_list, p.nodeCount()), isPlanar(unweighted_edge_list, test_graph.nodeCount()), edge_list.size(), seconds);

	return 0;
}
