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

typedef int node;
typedef std::pair<int, int> unweighted_edge;
typedef std::tuple<int, int, double> weighted_edge;
typedef std::list<unweighted_edge> unweighted_edge_list;
typedef std::list<weighted_edge> weighted_edge_list;

// is_planar is a wrapper for the boost library Boyer-Myrvold planarity test algorithm
// Parameters: an edge list pointer and the number of nodes in the graph
// Returns: 'true' if planar and 'false' if not planar
bool is_planar(Graph &graph) {

	unweighted_edge_list edge_list = graph.toEdgeList();
	boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> boost_graph(graph.nodeCount());

	for (unweighted_edge_list::iterator iter = edge_list.begin(); iter != edge_list.end(); iter++) {
		boost::add_edge((*iter).first, (*iter).second, boost_graph);
	}

	return boyer_myrvold_planarity_test(boost_graph);
}

// load_from_file loads a graph file with edges in the form 'node node weight' seperated by whitespace
// Parameters: A string with the path to the file
// Returns: A weighted edge list
weighted_edge_list load_from_file(std::string file_path) {

	weighted_edge_list edge_list;

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
	else {
		std::cout << "Cannot load " << file_path;
		exit(EXIT_FAILURE);
	}

	return edge_list;
}

// write_to_file writes the graph to a file
// Parameters: A weighted edge list, the initial number of edges, the number of seconds the filter
// algorithm took to execute, and the path to the file to write the graph to
void write_to_file(weighted_edge_list edge_list, int initial_edges, long seconds, std::string file_path ) {

	int a, b;
	double c;
	long hours, minutes;

	minutes = seconds / 60;
	hours = minutes / 60;

	int resultEdges = edge_list.size();
	float percentRetained = (float)resultEdges / (float)initial_edges * 100;


	std::ofstream file(file_path);

	file << "Execution time: " << hours << "h " << minutes % 60 << "m " << seconds % 60 << "s\n";
	file << "Initial number of edges: " << initial_edges << "\n";
	file << "Result number of edges: " << resultEdges << "\n";
	file << "Percent of edges retained: " << percentRetained << "%\n\n";

	for (weighted_edge_list::iterator iter = edge_list.begin(); iter != edge_list.end(); iter++) {
		a = std::get<0>(*iter);
		b = std::get<1>(*iter);
		c = std::get<2>(*iter);
		file << a << " " << b << " " << c << "\n";
	}

	file.close();
}

/*void maximizePlanar(unweighted_edge_list *edge_list, Graph *result_graph) {

	for (unweighted_edge_list::iterator iter = (*edge_list).begin(); iter != (*edge_list).end(); iter++) {
		edge_list_after.push_back(*iter);
		if (!isPlanar(edge_list_after)){
			(*graph).removeEdge(*iter);
		}
	}
}*/

int main(int argc, char* argv[]){

	if (argc < 3) {
		std::cout << "Please provide the path to the graph file to load as well as the path of the file to write the output to.\n";
	}

	weighted_edge_list initial_edge_list;
	unweighted_edge_list unweighted_initial_edge_list;
	Graph test_graph = Graph(0);

	std::string file_path = argv[1];

	initial_edge_list = load_from_file(file_path);

	weighted_edge_list temp = initial_edge_list;

	for (weighted_edge_list::iterator iter = initial_edge_list.begin(); iter != initial_edge_list.end(); iter++) {

		test_graph.addEdge(std::get<0>(*iter), std::get<1>(*iter)); // Initialize the adjacency list
		unweighted_initial_edge_list.push_back(unweighted_edge(std::get<0>(*iter), std::get<1>(*iter)));
	}

	assert("The provided file was empty.", initial_edge_list.size() < 1);

	if (is_planar(test_graph)) {
		std::cout << "The provided graph is already planar.\n";
		return 0;
	}

	std::clock_t begin = clock(); // Used to measure the execution time of the algorithm
	Graph result_graph = multipleComponentAlgorithmA(&test_graph); // Execute the planar graph filter on the graph
	clock_t end = clock(); // End execution measure

	long seconds = long(end - begin) / CLOCKS_PER_SEC; // Calculate the runtime in seconds

	weighted_edge_list result_weighted_edge_list;
	unweighted_edge_list result_edge_list = result_graph.toEdgeList();

	if (!is_planar(result_graph)){
		std::cout << "\nError! The result graph is not planar!";
		exit(EXIT_FAILURE);

	}

	std::cout << "Initial graph: " << test_graph.nodeCount() << " nodes";
	std::cout << "Result graph: " << result_graph.nodeCount() << " nodes";


	for (weighted_edge_list::iterator iter2 = initial_edge_list.begin(); iter2 != initial_edge_list.end(); iter2++) {
		if (std::find(result_edge_list.begin(), result_edge_list.end(), unweighted_edge(std::get<0>(*iter2), std::get<1>(*iter2))) != result_edge_list.end()) {
			result_weighted_edge_list.push_back(*iter2);
		}
	}

	write_to_file(result_weighted_edge_list, initial_edge_list.size(), seconds, argv[2]);

	return 0;
}
