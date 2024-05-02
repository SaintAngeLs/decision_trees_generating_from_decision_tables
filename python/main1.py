import argparse
import pandas as pd
from decision_tree import build_decision_tree, classify, DecisionNode

def parse_arguments():
    parser = argparse.ArgumentParser(description="Build and utilize a decision tree based on provided data.")
    parser.add_argument('data_file', type=str, help='Path to the CSV file containing the dataset.')
    return parser.parse_args()

def print_tree(node, depth=0):
    """Recursively print the tree structure in a readable format."""
    if node.is_leaf:
        print("\t" * depth + f"Leaf: {node.decision}")
    else:
        print("\t" * depth + f"Node: {node.attribute}")
        for value, child in node.children.items():
            print("\t" * depth + f"--> {value}:")
            print_tree(child, depth + 1)

def visualize_tree(node, graph, parent=None, edge_label=""):
    """Recursively visualize the tree using graphviz."""
    from graphviz import Digraph
    if node.is_leaf:
        node_label = f"Leaf: {node.decision}"
    else:
        node_label = f"Node: {node.attribute}"
    node_id = f"{id(node)}"  # Unique identifier for the graphviz node
    graph.node(node_id, label=node_label)
    if parent:
        graph.edge(parent, node_id, label=edge_label)
    if not node.is_leaf:
        for value, child in node.children.items():
            visualize_tree(child, graph, node_id, value)

def main():
    args = parse_arguments()
    
    # Load and display the data
    data = pd.read_csv(args.data_file)
    print("Dataset:")
    print(data)
    
    data_records = data.to_dict('records')
    

    attributes = ['age', 'income', 'student']  
    target_attribute = 'buys_computer'
    
    tree = build_decision_tree(data_records, attributes, target_attribute)
    
    print("\nDecision Tree Structure:")
    print_tree(tree)
    
    from graphviz import Digraph
    graph = Digraph()
    visualize_tree(tree, graph)

    graph.render('output/decision_tree_1', format='png', cleanup=True)
    print("Decision tree visualization saved as 'output/decision_tree.png'.")

    sample = {'age': 'youth', 'income': 'high', 'student': 'no'}
    decision = classify(tree, sample)
    print(f"\nDecision for the sample {sample} is: {decision}")

if __name__ == "__main__":
    main()
