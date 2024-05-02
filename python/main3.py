import json
import random
from graphviz import Digraph
from decision_tree import build_decision_tree, classify, DecisionNode

def generate_data(num_records):
    """ Generate complex employee data. """
    names = ["Robinson", "Warner", "Stevens", "White", "Smith", "Green", "Brown", "Klaus", "Houston", "Long", "Short", "Doe"]
    roles = ["Developer", "Manager", "Analyst", "Clerk", "Salesperson"]
    education_levels = ["Diploma", "Bachelor's", "Master's", "PhD"]
    performance_ratings = ["Below Average", "Average", "Above Average"]
    states = ["NJ", "FL", "CA", "TX", "NY"]
    marital_statuses = ["Married", "Single"]
    salary_bands = ["Low", "Medium", "High"]

    data = []
    for _ in range(num_records):
        name = random.choice(names)
        age = random.randint(21, 60)
        gender = random.choice(["Male", "Female"])
        children = random.randint(0, 4)
        education = random.choice(education_levels)
        role = random.choice(roles)
        experience = random.randint(1, 40)
        rating = random.choice(performance_ratings)
        state = random.choice(states)
        marital_status = random.choice(marital_statuses)
        salary = random.choice(salary_bands)

        data.append({
            "name": name,
            "age": age,
            "gender": gender,
            "marital_status": marital_status,
            "children": children,
            "education_level": education,
            "job_role": role,
            "years_of_experience": experience,
            "performance_rating": rating,
            "state": state,
            "salary_band": salary
        })
    return data

def print_tree(node, depth=0):
    """ Print the decision tree. """
    if node.is_leaf:
        print("\t" * depth + f"Leaf: {node.decision}")
    else:
        print("\t" * depth + f"Node: {node.attribute}")
        for value, child in node.children.items():
            print("\t" * depth + f"--> {value}:")
            print_tree(child, depth + 1)

def visualize_tree(node, graph, parent=None, edge_label=""):
    """Visualize the decision tree using Graphviz."""
    if node.is_leaf:
        node_label = f"Leaf: {node.decision}"
    else:
        node_label = f"Node: {node.attribute}"
    
    node_id = str(id(node))  # Ensuring node ID is always a string
    graph.node(node_id, label=node_label)
    
    if parent:
        # Converting edge_label to string to prevent type errors
        graph.edge(parent, node_id, label=str(edge_label))
    
    if not node.is_leaf:
        for value, child in node.children.items():
            # Ensuring value is passed as a string
            visualize_tree(child, graph, node_id, str(value))


def main():
    data_records = generate_data(100)  # Generate 100 employee records

    attributes = ["age", "gender", "marital_status", "children", "education_level", "job_role", "years_of_experience", "performance_rating", "state"]
    target_attribute = "salary_band"  # We will predict the salary band

    tree = build_decision_tree(data_records, attributes, target_attribute)
    print_tree(tree)

    graph = Digraph()
    visualize_tree(tree, graph)
    graph.render('output/complex_decision_tree', format='png', cleanup=True)
    print("Complex decision tree visualization saved as 'output/complex_decision_tree.png'.")

if __name__ == "__main__":
    main()
