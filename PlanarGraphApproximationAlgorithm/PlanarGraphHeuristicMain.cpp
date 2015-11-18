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

	//std::cout << "Checking planarity...\n";

	unweighted_edge_list edge_list = graph.toEdgeList(false);
	boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> boost_graph(graph.getNumNodes());

	for (unweighted_edge_list::iterator iter = edge_list.begin(); iter != edge_list.end(); iter++) {
		boost::add_edge((*iter).first, (*iter).second, boost_graph);
	}

	return boyer_myrvold_planarity_test(boost_graph);
}

// load_from_file loads a graph file with edges in the form 'node node weight' seperated by whitespace
// Parameters: A string with the path to the file
// Returns: A weighted edge list
weighted_edge_list load_from_file(std::string file_path) {

	std::cout << "Loading file...\n";

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
		std::cout << "\nCannot load " << file_path;
		exit(EXIT_FAILURE);
	}

	return edge_list;
}

// write_to_file writes the graph to a file
// Parameters: A weighted edge list, the initial number of edges, the number of seconds the filter
// algorithm took to execute, and the path to the file to write the graph to
void write_to_file(weighted_edge_list edge_list, std::string file_path ) {

	std::cout << "\nWriting result to file...\n";

	int a, b;
	double c;

	std::ofstream file(file_path);

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
		exit(EXIT_FAILURE);
	}

	weighted_edge_list input_edge_list;
	unweighted_edge_list unweighted_initial_edge_list;
	Graph input_graph = Graph();


	std::string file_path = argv[1];

	input_edge_list = load_from_file(file_path);

	weighted_edge_list temp = input_edge_list;

	for (weighted_edge_list::iterator iter = input_edge_list.begin(); iter != input_edge_list.end(); iter++) {

		input_graph.addEdge(std::get<0>(*iter), std::get<1>(*iter)); // Initialize the adjacency list
		unweighted_initial_edge_list.push_back(unweighted_edge(std::get<0>(*iter), std::get<1>(*iter)));
	}

	input_graph.trim();

	if (input_edge_list.size() < 1){
		std::cout << "The provided file is empty";
		exit(EXIT_FAILURE);
	}

	if (is_planar(input_graph)) {
		std::cout << "The provided graph is already planar.\n";
		exit(EXIT_SUCCESS);
	}

	std::clock_t begin = clock(); // Used to measure the execution time of the algorithm
	Graph result_graph = multipleComponentAlgorithmA(&input_graph); // Execute the planar graph filter on the graph
	clock_t end = clock(); // End execution measure

	long seconds = long(end - begin) / CLOCKS_PER_SEC; // Calculate the runtime in seconds
	long hours, minutes;

	minutes = seconds / 60;
	hours = minutes / 60;

	weighted_edge_list result_weighted_edge_list;
	unweighted_edge_list result_edge_list = result_graph.toEdgeList(false);

	if (!is_planar(result_graph)){
		std::cout << "\nError! The result graph is not planar!";
		exit(EXIT_FAILURE);

	}

	int input_num_edges = input_graph.getNumEdges();
	int result_num_edges = result_graph.getNumEdges();


	std::cout << "\nExecution time: " << hours << "h " << minutes % 60 << "m " << seconds % 60 << "s";

	std::cout << "\n\nInitial graph:";
	std::cout << "\nNodes: " << input_graph.getNumNodesWithEdges();
	std::cout << "\nEdges: " << input_num_edges;

	std::cout << "\n\nResult graph:";
	std::cout << "\nNodes: " << result_graph.getNumNodesWithEdges();
	std::cout << "\nEdges: " << result_num_edges;

	std::cout << "\n\nPercent of edges retained: " << (float)result_num_edges / (float)input_num_edges * 100 << "%";



	for (weighted_edge_list::iterator iter2 = input_edge_list.begin(); iter2 != input_edge_list.end(); iter2++) {
		if (std::find(result_edge_list.begin(), result_edge_list.end(), unweighted_edge(std::get<0>(*iter2), std::get<1>(*iter2))) != result_edge_list.end()) {
			result_weighted_edge_list.push_back(*iter2);
		}
	}

	write_to_file(result_weighted_edge_list, argv[2]);

	std::cin.get();

	return 0;
}
