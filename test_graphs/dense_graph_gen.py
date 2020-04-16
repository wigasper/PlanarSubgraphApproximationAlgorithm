import random
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--nodes", help="Approximate (subject to floor division) "  
                "total number of nodes in graph", required=True, type=int)
    parser.add_argument("-c", "--components", help="Approximate " 
                "total number of components in graph", required=True, type=int)
    args = parser.parse_args()

    nodes_per_component = int(args.nodes / args.components)

    with open(f"{args.nodes}_node_{args.components}_comp_dense_graph", "w") as out:
        naming_offset = 0
        
        for _ in range(args.components):
            for node_id in range(nodes_per_component):
                adjacents = [random.randint(0, nodes_per_component - 1) for _ in range(int(nodes_per_component / 2))]
                adjacents = list(dict.fromkeys(adjacents))
                for adj in adjacents:
                    out.write(" ".join([str(node_id + naming_offset), str(adj + naming_offset), "1"]))
                    out.write("\n")
            naming_offset += nodes_per_component

if __name__ == "__main__":
    main()
