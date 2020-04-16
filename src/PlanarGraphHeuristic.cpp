// This work is heavily comprised of Nathan Cornelius's original code from
// github.com/ndcornelius/PlanarSubgraphApproximationAlgorithm

// TODO: honestly may need to get rid of many or all lists to make everything easier

#include "AdjacencyList.cpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <unordered_set>

#include <omp.h>

enum VisitedState { VISITED, QUEUED, UNVISITED };
typedef int node;
typedef std::pair<node, node> edge;

// Takes a start node and a graph and returns the list of nodes that are
//  in the same component as the start node.
std::list<node> nodeBFS(node startNode, Graph *graph) {

    std::queue<node> nodeQueue;
    std::map<node, VisitedState> visited;
    std::list<node> nodeList = (*graph).getNodes();

    for (node i : nodeList) {
	visited.insert({i, UNVISITED});
    }

    std::list<node> component;

    nodeQueue.push(startNode);

    while (!nodeQueue.empty()) {
	int nodeQueueSize = nodeQueue.size();

	for (int i = 0; i < nodeQueueSize; i++) {
	    node currentNode;
	    currentNode = nodeQueue.front();

	    nodeQueue.pop();

	    if (visited[currentNode] != VISITED) {
		std::list<node> *adj;

		visited[currentNode] = VISITED;
		component.push_back(currentNode);
		adj = (*graph).getList(currentNode);

		std::vector<node> adjs_vec(std::begin(*adj), std::end(*adj));
		for (node j : adjs_vec) {
		    if (visited[j] == UNVISITED) {
			visited[j] = QUEUED;
			nodeQueue.push(j);
		    }
		}
	    }
	}
    }

    return component;
}

// getComponents takes a graph and returns a std::vector of std::list that are
// each a list of the nodes in seperate components in the graph.
std::vector<std::list<node>> getComponents(Graph *graph) {

    std::unordered_map<node, VisitedState> visited;
    std::list<node> nodeList = (*graph).getNodes();

    for (node i : nodeList) {
	visited.insert({i, UNVISITED});
    }

    std::vector<std::list<node>> components; // The list of components

    bool unvisited = true;
    node element = (*graph).first();

    while (unvisited) { // While there is still an unvisited node.
	std::list<node> component = nodeBFS(element, graph);

	components.push_back(component);

	for (node j : component) {
	    visited[j] = VISITED;
	}

	unvisited = false;

	for (auto &[key_node, state] : visited) {
	    if (state == UNVISITED) {
		unvisited = true;
		element = key_node;
		break;
	    }
	}
    }

    return components;
}

// TODO: review this - maybe issue with using a vector indices to map to visited state
std::list<edge> intercomponentSpanningTree(Graph *graph,
					   std::vector<std::list<node>> *componentList) {

    std::vector<std::list<node>> components = *componentList;

    std::unordered_map<node, int> nodeToComponent;

    for (std::size_t i = 0; i < components.size(); i++) {
	for (node node_id : components[i]) {
	    nodeToComponent[node_id] = i;
	}
    }

    std::queue<int> componentQueue;
    std::vector<VisitedState> visited(components.size(), UNVISITED);
    std::list<edge> edge_list;
    componentQueue.push(0);

    while (!componentQueue.empty()) {
	int currentComponent = componentQueue.front();
	componentQueue.pop();

	if (visited.at(currentComponent) != VISITED) {
	    visited[currentComponent] = VISITED;

	    std::list<node> component = components.at(currentComponent);

	    for (node node1 : component) {
		std::list<node> *adj = (*graph).getList(node1);

		for (node node2 : *adj) {
		    int newComponent = nodeToComponent[node2];
		    // If the edge is intercomponent and the new component is unvisited
		    if (visited.at(newComponent) == UNVISITED && currentComponent != newComponent) {
			visited[newComponent] = QUEUED;
			edge_list.push_back(edge(node1, node2));
			componentQueue.push(newComponent);
		    }
		}
	    }
	}
    }

    return edge_list;
}

// Bounded degree graph
// The greedy maximal planar subgraph algorithm
Graph algorithmA(Graph *graph, bool multicomponent) {

    const int v = (*graph).getNumNodes();

    Graph E1 = Graph();
    std::list<node> active;

    // This is for the unique case where the original graph file
    // had a single node whose only edge was from itself to itself.
    if (v == 1) {
	E1.addEdge((*graph).first(), (*graph).first());
	return E1;
    }

    std::list<node> nodeList = (*graph).getNodes();
    std::vector<node> nodeVec(std::begin(nodeList), std::end(nodeList));

    for (node i : nodeList) {
	E1.addNode(i);
    }

    std::unordered_set<node> nu(std::begin(nodeList), std::end(nodeList));

    // init with distant nodes in the vec. a better way to do this
    // would be to find nodes that are actually distant on the graph
    for (int i = 1; i < 9; i++) {
	size_t idx = (nodeVec.size() - 1) / i;
	node node_to_add = nodeVec.at(idx);
	active.push_front(node_to_add);
	nu.erase(node_to_add);
    }

    while (!nu.empty()) {
	// If active is empty take the front element from nu and add to active
	if (active.empty()) {
	    node temp = *nu.begin();
	    active.push_front(temp);
	    nu.erase(temp);
	}

	// copy list to vec for OpenMP
	std::vector<node> activeVec(std::begin(active), std::end(active));

	active.clear();
	// Currently 2 threads may be fastest here, otherwise threads get locked up
	// waiting for critical sections
	#pragma omp parallel for num_threads(2) if (!multicomponent)
	for (node x : activeVec) {
	    // Get the adjacency list for the chosen node x
	    std::list<node> *xList = (*graph).getList(x);

	    std::unordered_set<node> aux(std::begin(*xList), std::end(*xList));

	    // Go through the adjacency list for x
	    for (node i : *xList) {
		node y = i;

		// if y is in nu
		bool find_result_y = false;
		bool y_added_to_triangle = false;
		#pragma omp critical(nu)
		{
		    auto search = nu.find(y);
		    find_result_y = (search != nu.end());
		    if (find_result_y) {
			nu.erase(y);
		    }
		}

		if (find_result_y) {
		    // Get the adjacency list for y
		    std::list<node> *yList = (*graph).getList(y);

		    // Go through the adjacency list for y
		    bool break_flag = false;
		    for (node j : *yList) {
			if (!break_flag) {
			    node z = j;

			    // this checks to see if z has an edge with x
			    // no point in continuing if not
			    auto aux_search = aux.find(z);

			    if (aux_search != aux.end()) {
				// Check if z is in nu
				bool find_result_z = false;
				bool z_added_to_triangle = false;
				#pragma omp critical(nu)
				{
				    auto search = nu.find(z);
				    find_result_z = (search != nu.end());
				    if (find_result_z) {
					nu.erase(z);
				    }
				}

				if (find_result_z) {
				    // Add triangle (x,y,z) to E1
				    E1.addEdge(y, z);
				    E1.addEdge(x, z);
				    E1.addEdge(y, x);

				    #pragma omp critical(active)
				    {
					active.push_front(y);
					active.push_front(z);
				    }

				    y_added_to_triangle = true;
				    z_added_to_triangle = true;

				    // Set y and x to 0 in aux because we can't use them to make
				    // any more triangles
				    aux.erase(y);
				    aux.erase(z);

				    // Break out of loop thru y since we can't use y to make any
				    // more triangles
				    // can't use 'break' keyword with openmp
				    break_flag = true;

				    if (!z_added_to_triangle) {
					#pragma omp critical(nu)
					nu.insert(z);
				    }
				}
			    }
			}
		    }

		    if (!y_added_to_triangle) {
			#pragma omp critical(nu)
			nu.insert(y);
		    }
		}
	    } // End while loop through node x adjacency list
	}
    } // End while loop through nu

    // Find the components in the graph
    std::vector<std::list<node>> components = getComponents(&E1);

    std::list<edge> componentSpanningTree = intercomponentSpanningTree(graph, &components);

    for (edge cst_edge : componentSpanningTree) {
	E1.addEdge(cst_edge);
    }

    return E1;
} // End algorithmA

Graph multipleComponentAlgorithmA(Graph *graph) {
    omp_set_nested(1);

    Graph planarGraph = Graph();

    // a vector of lists that contain the nodes in the seperate components
    std::vector<std::list<node>> componentNodeList = getComponents(graph);

    bool multiple_components = false;
    if (componentNodeList.size() > 1) {
	multiple_components = true;
    }

    std::vector<Graph> components;

    for (std::list<node> compNodes : componentNodeList) {
	Graph component = Graph();

	for (node node1 : compNodes) {
	    Graph().addNode(node1);
	    std::list<node> *adj = (*graph).getList(node1);
	    // go through each node's adjacency list and add edges to component graph
	    for (node node2 : *adj) {
		component.addEdge(node1, node2);
	    }
	}
	components.push_back(component);
    }

    Graph result_graph = Graph();

    #pragma omp parallel for if (multiple_components)
    for (Graph &component : components) {
	Graph component_result = algorithmA(&component, multiple_components);
	std::list<edge> comp_edge_list = component_result.toEdgeList();
	for (edge comp_edge : comp_edge_list) {
	    result_graph.addEdge(comp_edge);
	}
    }

    return result_graph;
}
