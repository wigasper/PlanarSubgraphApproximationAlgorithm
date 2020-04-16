import argparse

from networkx.algorithms.community import LFR_benchmark_graph

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--nodes", help="Approximate (subject to floor division) "  
                "total number of nodes in graph", required=True, type=int)
    parser.add_argument("-c", "--components", help="Approximate (subject to LFR algorithm) " 
                "total number of components in graph", required=True, type=int)
    args = parser.parse_args()

    nodes_per_graph = int(args.nodes / args.components)

    graphs = []
    for _ in range(args.components):
        n = nodes_per_graph
        tau1 = 3
        tau2 = 1.5
        mu = 0.1
        graph = LFR_benchmark_graph(n, tau1, tau2, mu, average_degree=5, min_community=20, 
                                seed=42, max_iters=500)
        graphs.append(graph)

    with open(f"{args.nodes}_node_{args.components}_component_graph", "w") as out:
        # this is used to create the separate components by ensuring that the nodes 
        # in each graph have individual names
        naming_offset = 0

        for graph in graphs:
            for edge in graph.edges:
                out.write(" ".join([str(edge[0] + naming_offset), str(edge[1] + naming_offset), 
                    "1", "\n"]))
            naming_offset += len(graph.nodes)

if __name__ == "__main__":
    main()
