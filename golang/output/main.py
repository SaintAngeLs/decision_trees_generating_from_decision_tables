import json
from graphviz import Digraph

def visualize_tree(node, graph, parent=None, edge_label=""):
    """Visualize the decision tree using Graphviz."""
    if node["is_leaf"]:
        node_label = f"Leaf: {node["decision"]}"
    else:
        node_label = f"Node: {node["attribute"]}"
    
    node_id = str(id(node))  # Ensuring node ID is always a string
    graph.node(node_id, label=node_label)
    
    if parent:
        # Converting edge_label to string to prevent type errors
        graph.edge(parent, node_id, label=str(edge_label))
    
    if not node["is_leaf"]:
        for value, child in node["children"].items():
            # Ensuring value is passed as a string
            visualize_tree(child, graph, node_id, str(value))

def main():
    with open("output/json/comp_decision_tree.json") as f:
        tree = json.load(f)
        graph = Digraph()
        visualize_tree(tree, graph)
        graph.render("output/comp_decision_tree", format="png", cleanup=True)

main()