
#include <vector>
#include "AdjacencyList.cpp"
#include <list>
#include <algorithm>
#include <queue>
#include <tuple>
#include <map>
#include <iostream>

enum VisitedState { VISITED, QUEUED, UNVISITED };
typedef int node;
typedef std::pair<node, node> edge;


// Takes a start node and a graph and returns the list of nodes that are 
//  in the same component as the start node.
std::list<node> nodeBFS(node startNode, Graph *graph) {

	std::queue<node> nodeQueue;
	std::map<node, VisitedState> visited;
	std::list<node> nodeList = (*graph).getNodes();

	for (std::list<node>::iterator i = nodeList.begin(); i != nodeList.end(); i++) {
		visited[*i] = UNVISITED;
	}

	std::list<node> component;

	nodeQueue.push(startNode);

	while (!nodeQueue.empty()) { // nodeQueue will be empty when a complete component is found
		node currentNode = nodeQueue.front();
		nodeQueue.pop();

		if (visited[currentNode] != VISITED){
			visited[currentNode] = VISITED;
			component.push_back(currentNode);
			std::list<node> *adj = (*graph).getList(currentNode);

			for (std::list<node>::iterator iter = (*adj).begin(); iter != (*adj).end(); iter++){ // Go through the adjacency list
				if (visited[*iter] == UNVISITED) {
					visited[*iter] = QUEUED;
					nodeQueue.push(*iter);
				}
			}
		}
	}

	return component;
}

// getComponents takes a graph and returns a std::vector of std::list that are each a list of 
// the nodes in seperate components in the graph.
std::vector<std::list<node>> getComponents(Graph *graph) {

	std::map<node, VisitedState> visited;
	std::list<node> nodeList = (*graph).getNodes();

	for (std::list<node>::iterator i = nodeList.begin(); i != nodeList.end(); i++) {
		visited[*i] = UNVISITED;
	}

	std::vector<std::list<node>> components; // The list of components

	bool unvisited = true;
	node element = (*graph).first();

	while (unvisited) { // While there is still an unvisited node.
		std::list<node> component = nodeBFS(element, graph);

		components.push_back(component);

		for (std::list<node>::iterator iter = component.begin(); iter != component.end(); iter++){
			visited[*iter] = VISITED;
		}

		unvisited = false;

		for (std::map<node, VisitedState>::iterator i = visited.begin(); i != visited.end(); i++) {
			if ((*i).second == UNVISITED) {
				unvisited = true;
				element = (*i).first;
				break;
			}
		}
	}

	return components;
}

// Takes a start node and a graph and returns a spanning tree of the component hat the
// start node is in. If the graph is fully connected, this returns a completespanning
// tree of the graph.
std::list<std::pair<node, node>> edgeBFS(node startNode, Graph *graph) { 

	std::queue<node> nodeQueue;
	std::vector<VisitedState> visited((*graph).getNumNodes(), UNVISITED);
	std::list<edge> edge_list;
	nodeQueue.push(startNode);

	while (!nodeQueue.empty()) { // nodeQueue will be empty when every node has been visited
		node currentNode = nodeQueue.front();
		nodeQueue.pop();

		if (visited[currentNode] != VISITED) {
			visited[currentNode] = VISITED;

			std::list<node> *adj = (*graph).getList(currentNode);

			for (std::list<node>::iterator iter = (*adj).begin(); iter != (*adj).end(); iter++){ // Go through the adjacency list
				if (visited[*iter] == UNVISITED) {
					visited[*iter] = QUEUED;
					edge_list.push_back(edge (currentNode, *iter));
					nodeQueue.push(*iter);
				}
			}
		}
	}

	return edge_list;
}

std::list<edge> intercomponentSpanningTree(Graph *graph, std::vector<std::list<node>> *componentList) {

	std::vector<std::list<node>> components = *componentList;

	std::map<int, int> nodeToComponent; // Used to map a node to it's component number in order to facilitate a fast lookup of the component any given node is in

	for (std::size_t i = 0; i < components.size(); i++){ // Create the map from node to component
		for (std::list<int>::iterator iter = components[i].begin(); iter != components[i].end(); iter++){
			nodeToComponent[*iter] = i;
		}
	}

	std::queue<int> componentQueue;
	std::vector<VisitedState> visited(components.size(), UNVISITED);
	std::list<edge> edge_list;
	componentQueue.push(0);

	while (!componentQueue.empty()) { // componentQueue will be empty when every component has been visited
		int currentComponent = componentQueue.front();
		componentQueue.pop();

		if (visited[currentComponent] != VISITED) {
			visited[currentComponent] = VISITED;

			std::list<node> component = components[currentComponent];

			for (std::list<node>::iterator node1 = component.begin(); node1 != component.end(); node1++){ // Go through the component
				std::list<node> *adj = (*graph).getList(*node1);

				for (std::list<node>::iterator node2 = (*adj).begin(); node2 != (*adj).end(); node2++){ // Go through the adjacency list
					int newComponent = nodeToComponent[*node2];

					if (visited[newComponent] == UNVISITED && currentComponent != newComponent) { // If the edge is intercomponent and the new component is invisited
						visited[newComponent] = QUEUED;
						edge_list.push_back(edge(*node1, *node2));
						componentQueue.push(newComponent);
					}
				}
			}
		}
	}

	return edge_list;

}

// Bounded degree graph
Graph algorithmA(Graph *graph) { // The greedy maximal planar subgraph algorithm.

	std::cout << "\n\nNum nodes in component: " << (*graph).getNumNodes();

	const int v = (*graph).getNumNodes(); //Get the number of nodes in the graph

	Graph E1 = Graph();
	std::list<int> nu;
	std::list<int> active;
	std::list<int> used;

	std::list<node> nodeList = (*graph).getNodes();
	for (std::list<node>::iterator i = nodeList.begin(); i != nodeList.end(); ++i) {
		E1.addNode((*i));
	}

	for (int i = 0; i < v; i++) {// Initialize the nu array with all nodes
		nu.push_front(i);
	}

	while (!nu.empty()) { // While there are nodes in nu

		std::vector<int> aux(v, 0); // Initialize the auxiliary array

		if (active.empty()) { // If active is empty take the front element from nu and add it to active.
			active.push_front(nu.back());
			nu.pop_back();
		}

		// When there is an active vertex

		int x = active.front(); // Choose an active vertex to "use".
		active.pop_front();

		std::list<int> *xList = (*graph).getList(x); // Get the adjacenecy list for the chosen node x.

		for (std::list<int>::iterator iter = (*xList).begin(); iter != (*xList).end(); iter++) { // Set up the auxiliary array
			aux[*iter] = 1;
		}

		for (std::list<int>::iterator i = (*xList).begin(); i != (*xList).end(); i++) { // Go through the adjacency list for x
			int y = *i;

			if (std::find(nu.begin(), nu.end(), y) != nu.end()) { // If y is in new

				std::list<int> *yList = (*graph).getList(y); // Get the adjacency list for y

				for (std::list<int>::iterator j = (*yList).begin(); j != (*yList).end(); j++) { // Go through the adjacency list for y.
					int z = *j;

					if (std::find(nu.begin(), nu.end(), z) != nu.end()) { // check if z is in new

						if (aux[z] == 1) { // Check the auxilliary array to see if y is has an edge with z
											//If so
							E1.addEdge(y, z); // add triangle (x, y, z) to E1.
							E1.addEdge(x, z);
							E1.addEdge(y, x);
							
							active.push_front(y);
							nu.remove(y); // Mark y and z as active and remove from nu

							active.push_front(z);
							nu.remove(z);

							aux[y] = 0; //Set y and x to 0 in the auxiliary array becaus we can't use them to make any more triangles
							aux[z] = 0;

							break; // Break out of the loop through y since we can't use y to make any more triangles
						}
					}
				} // End while loop through node y adjacency ist
			} 
		} // End while loop through node x adjacency list

		used.push_front(x);

	} // End while loop through nu


	std::vector<std::list<int>> components = getComponents(&E1); // Find the components in the graph

	std::list<edge> componentSpanningTree = intercomponentSpanningTree(graph, &components);
	
	for (std::list<edge>::iterator iter = componentSpanningTree.begin(); iter != componentSpanningTree.end(); iter++){ // Go through the spanning tree edge list
			E1.addEdge(*iter); // Add the edge to the graph
	}

	std::cout << "\nNum nodes in result: " << E1.getNumNodesWithEdges() << "\n\n";

	return E1;
}// End algorithmA

Graph multipleComponentAlgorithmA(Graph *graph) {

	std::cout << "\nExtracting components from input graph...";

	Graph planarGraph = Graph(); // Initialize the return graph

	std::vector<std::list<node>> componentNodeList = getComponents(graph); //	A vector of lists that contain the nodes in the seperate components
	std::list<Graph> components;

	for (std::vector<std::list<node>>::iterator compNodes = componentNodeList.begin(); compNodes != componentNodeList.end(); compNodes++){ // Iterate through the list of components
		Graph component = Graph();

		for (std::list<node>::iterator node1 = (*compNodes).begin(); node1 != (*compNodes).end(); node1++){ // Iterate through the list of nodes in the component
			Graph().addNode(*node1); // For the case when the component is a single node.
			std::list<node> *adj = (*graph).getList((*node1));
			for (std::list<node>::iterator node2 = (*adj).begin(); node2 != (*adj).end(); node2++){ // Iterate through each node's adjacency list
				component.addEdge(*node1, *node2); // Add the edges to the component graph
			}
		}
		components.push_back(component);
	}

	std::cout << "\nRunning algorithm A on " << components.size() << " component(s).";

	for (std::list<Graph>::iterator component = components.begin(); component != components.end(); component++){
		Graph g = algorithmA(&((*component))); // Run the algorithm on each component

		std::list<edge> edge_list = g.toEdgeList();

		for (std::list<edge>::iterator edge = edge_list.begin(); edge != edge_list.end(); edge++){
			planarGraph.addEdge(*edge);
		}
	}
	
	return planarGraph;
}