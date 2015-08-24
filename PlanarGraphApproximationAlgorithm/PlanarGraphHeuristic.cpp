
#include <vector>
#include "AdjacencyList.cpp"
#include <list>
#include <algorithm>
#include <queue>
#include <tuple>
#include <map>

enum NodeState { VISITED, QUEUED, UNVISITED };
typedef int node;
typedef std::pair<node, node> edge;



// Takes a start node and a graph and returns the list of nodes that are 
//  in the same component as the start node.
std::list<node> nodeBFS(node startNode, Graph graph) {

	std::queue<node> nodeQueue;
	std::vector<NodeState> visited(graph.nodeCount(), UNVISITED);
	std::list<node> component;

	nodeQueue.push(startNode);

	while (!nodeQueue.empty()) { // nodeQueue will be empty when a complete component is found
		node currentNode = nodeQueue.front();
		nodeQueue.pop();

		if (visited[currentNode] != VISITED){
			visited[currentNode] = VISITED;
			component.push_back(currentNode);
			std::list<node> adj = *graph.getList(currentNode);

			for (std::list<node>::iterator iter = adj.begin(); iter != adj.end(); iter++){ // Go through the adjacency list
				if (visited[*iter] == UNVISITED) {
					visited[*iter] = QUEUED;
					component.push_back(*iter);
					nodeQueue.push(*iter);
				}
			}
		}
	}

	return component;
}

// getComponents takes a graph and returns a std::vector of std::list that are each a list of 
// the nodes in seperate components in the graph.
std::vector<std::list<node>> getComponents(Graph graph) {

	std::vector<NodeState> visited(graph.nodeCount(), UNVISITED); // The vector that signifies the traversal state of each node
	std::vector<std::list<node>> components; // The list of components

	std::vector<NodeState>::iterator unvisited;

	while ((unvisited = std::find(visited.begin(), visited.end(), UNVISITED)) != visited.end()) { // While there is still an unvisited node.
		std::list<node> component = nodeBFS(unvisited - visited.begin(), graph);

		components.push_back(component);

		for (std::list<node>::iterator iter = component.begin(); iter != component.end(); iter++){
			visited[*iter] = VISITED;
		}
	}

	return components;
}

// Takes a start node and a graph and returns a spanning tree of the component hat the
// start node is in. If the graph is fully connected, this returns a completespanning
// tree of the graph.
std::list<std::pair<node, node>> edgeBFS(node startNode, Graph graph) { 

	std::queue<node> nodeQueue;
	std::vector<NodeState> visited(graph.nodeCount(), UNVISITED);
	std::list<std::pair<node, node>> edge_list;
	nodeQueue.push(startNode);

	while (!nodeQueue.empty()) { // nodeQueue will be empty when every node has been visited
		node currentNode = nodeQueue.front();
		nodeQueue.pop();

		if (visited[currentNode] != VISITED) {
			visited[currentNode] = VISITED;

			std::list<node> adj = *graph.getList(currentNode);

			for (std::list<node>::iterator iter = adj.begin(); iter != adj.end(); iter++){ // Go through the adjacency list
				if (visited[*iter] == UNVISITED) {
					visited[*iter] = QUEUED;
					edge_list.push_back(std::pair<node, node> (currentNode, *iter));
					nodeQueue.push(*iter);
				}
			}
		}
	}

	return edge_list;
}

// Bounded degree graph
Graph algorithmA(Graph graph) { // The greedy maximal planar subgraph algorithm.

	const int v = graph.nodeCount(); //Get the number of nodes in the graph

	Graph E1 = Graph(v);
	std::list<int> nu;
	std::list<int> active;
	std::list<int> used;

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

		std::list<int> xList = *graph.getList(x); // Get the adjacenecy list for the chosen node x.

		for (std::list<int>::iterator iter = xList.begin(); iter != xList.end(); iter++) { // Initialize the auxiliary array
			aux[*iter] = 1;
		}

		for (std::list<int>::iterator i = xList.begin(); i != xList.end(); i++) { // Go through the adjacency list for x
			int y = *i;

			if (find(nu.begin(), nu.end(), y) != nu.end()) { // Check if y is in new

				std::list<int> yList = *graph.getList(y); // Get the adjacency list for y

				for (std::list<int>::iterator j = yList.begin(); j != yList.end(); j++) { // Go through the adjacency list for y.
					int z = *j;

					if (find(nu.begin(), nu.end(), z) != nu.end()) { // check if z is in new

						if (aux[z] == 1) { // If node y has a neghbor z in the auxiliary array 

							E1.addEdge(y, z); // add triangle (x, y, z) to E1.
							E1.addEdge(x, z);
							E1.addEdge(y, x);

							active.push_front(y);
							nu.remove(y); // Mark y and z as active and remove from nu

							active.push_front(z);
							nu.remove(z);

							aux[y] = 0;
							aux[z] = 0;
						}
					}
					if (find(nu.begin(), nu.end(), y) == nu.end()) {
						break;
					}
				}
			} // End while loop through node y adjacency ist
		}// End while loop through node x adjacency list

		used.push_front(x);

	} // End while loop through nu



	std::vector<std::list<int>> components;
	components = getComponents(E1); // Find the components in the graph



	std::map<int, int> nodeToComponent; // Used to map a node to it's component number in order to facilitate a fast lookup of the component any given node is in

	for (std::size_t i = 0; i < components.size(); i++){ // Create the map from node to component
		for (std::list<int>::iterator iter = components[i].begin(); iter != components[i].end(); iter++){
			nodeToComponent[*iter] = i;
		}
	}


	std::map<edge, edge> edgesH;
	Graph H(components.size());

	for (std::size_t current_component = 0; current_component < components.size(); current_component++){ // Go through each component
		for (std::list<node>::iterator current_node = components[current_component].begin(); current_node != components[current_component].end(); current_node++){ // Go through each node in the current componenet
			std::list<int> adj = *graph.getList(*current_node); // Get the adjacency list for the node

			for (std::list<int>::iterator iter = adj.begin(); iter != adj.end(); iter++) { // Go through the adjacency list

				if (nodeToComponent[*iter] != current_component) { // If the adjacent node is not in the current node's component
					H.addEdge(current_component, nodeToComponent[*iter]); // Add the edge
					edge h_edge(current_component, nodeToComponent[*iter]);
					edge e1_edge(*current_node, *iter);
					edgesH[h_edge] = e1_edge; // Map the H edge to the E1 edge
				}
			}
		}
	}

	std::list<edge> spanning_tree = edgeBFS(0, H);

	for (std::list<edge>::iterator iter = spanning_tree.begin(); iter != spanning_tree.end(); iter++) {
		E1.addEdge(edgesH[*iter]);
	}

	return E1;
}// End algorithmA