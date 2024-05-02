import argparse
import json
from graphviz import Digraph
from decision_tree import build_decision_tree, classify, DecisionNode

class DataLoader:
    def load_data(self):
        raise NotImplementedError("Subclasses must implement this method")

class JSONDataLoader(DataLoader):
    def __init__(self, json_str):
        self.json_str = json_str

    def load_data(self):
        data = json.loads(self.json_str)
        employees = data['company']['employees']
        flat_data = []
        for emp in employees:
            states = {loc['state'] for loc in emp['locations']}
            zip_codes = {loc['zipCode'] for loc in emp['locations']}
            emp['states'] = ', '.join(states)
            emp['zip_codes'] = ', '.join(zip_codes)
            emp['multi_location'] = len(emp['locations']) > 1
            del emp['locations']
            flat_data.append(emp)
        return flat_data

class DataLoaderFactory:
    def get_loader(data_type, source):
        if data_type == "json":
            return JSONDataLoader(source)
        else:
            raise ValueError("Unsupported data type")

def print_tree(node, depth=0):
    if node.is_leaf:
        print("\t" * depth + f"Leaf: {node.decision}")
    else:
        print("\t" * depth + f"Node: {node.attribute}")
        for value, child in node.children.items():
            print("\t" * depth + f"--> {value}:")
            print_tree(child, depth + 1)

def visualize_tree(node, graph, parent=None, edge_label=""):
    if node.is_leaf:
        node_label = f"Leaf: {node.decision}"
    else:
        node_label = f"Node: {node.attribute}"
    node_id = str(id(node))
    graph.node(node_id, label=node_label)
    if parent:
        graph.edge(parent, node_id, label=str(edge_label))
    if not node.is_leaf:
        for value, child in node.children.items():
            visualize_tree(child, graph, node_id, str(value))

def main():
    parser = argparse.ArgumentParser(description="Process and analyze data.")
    parser.add_argument('data_type', choices=['json'], help='The format of the input data.')
    parser.add_argument('file_path', help='File path of the JSON data.')
    args = parser.parse_args()

    with open(args.file_path, 'r') as file:
        json_str = file.read()

    loader = DataLoaderFactory.get_loader(args.data_type, json_str)
    data_records = loader.load_data()

    scenarios = [
    {'name': 'salary', 'attributes': ['age', 'states', 'children'], 'target': 'salary', 'description': 'Analyzing how age, state diversity, and number of children affect salary ranges.'},
    {'name': 'marital_children', 'attributes': ['maritalStatus', 'children'], 'target': 'salary', 'description': 'Exploring the impact of marital status and number of children on salary levels.'},
    {'name': 'state_employment', 'attributes': ['states'], 'target': 'maritalStatus', 'description': 'Studying state distribution and its correlation with marital status among employees.'},
    {'name': 'complex_profile', 'attributes': ['age', 'states', 'children', 'maritalStatus', 'multi_location', 'zip_codes'], 'target': 'salary', 'description': 'A comprehensive analysis examining how age, state diversity, number of children, marital status, multiple locations, and various zip codes influence salary.'}
]


    for scenario in scenarios:
        print(f"\nScenario: {scenario['name']}")
        print(f"Description: {scenario['description']}")
        attributes = scenario['attributes']
        target_attribute = scenario['target']
        tree = build_decision_tree(data_records, attributes, target_attribute)
        print_tree(tree)

        graph = Digraph()
        visualize_tree(tree, graph)
        graph.render(f'output/decision_tree_{scenario["name"]}', format='png', cleanup=True)
        print(f"Decision tree visualization for {scenario['name']} saved as 'output/decision_tree_{scenario['name']}.png'.")

if __name__ == "__main__":
    main()
