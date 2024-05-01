class DecisionNode:
    """A node in the decision tree, which may be either a decision node or a leaf node."""
    def __init__(self, attribute=None, is_leaf=False, decision=None):
        self.attribute = attribute  # Attribute to split on at this node
        self.is_leaf = is_leaf      # Whether this is a leaf node
        self.decision = decision    # Decision if this is a leaf
        self.children = {}          # Dictionary to hold children nodes: attribute_value -> DecisionNode

def choose_best_attribute(data, attributes, target_attribute):
    """Selects the attribute that best splits the data using a simple count of the most frequent outcome."""
    best_attribute = None
    max_gain = 0
    
    for attribute in attributes:
        gain = information_gain(data, attribute, target_attribute)
        if gain > max_gain:
            max_gain = gain
            best_attribute = attribute
    
    return best_attribute

def information_gain(data, attribute, target_attribute):
    """Calculates the information gain of an attribute."""
    value_counts = {}
    attribute_entropy = 0.0

    # Calculate the frequency of each value in the target attribute
    for row in data:
        value = row[attribute]
        target_value = row[target_attribute]
        if value not in value_counts:
            value_counts[value] = {}
        if target_value not in value_counts[value]:
            value_counts[value][target_value] = 0
        value_counts[value][target_value] += 1

    # Calculate the entropy of each subset of records weighted by their probability.
    total_rows = len(data)
    for value in value_counts:
        value_entropy = 0
        total_value_rows = sum(value_counts[value].values())
        for outcome_count in value_counts[value].values():
            probability = outcome_count / total_value_rows
            value_entropy -= probability * log2(probability)
        attribute_entropy += (total_value_rows / total_rows) * value_entropy
    
    # Calculate the entropy before the split
    overall_entropy = 0
    target_value_counts = {}
    for row in data:
        target_value = row[target_attribute]
        if target_value not in target_value_counts:
            target_value_counts[target_value] = 0
        target_value_counts[target_value] += 1
    for count in target_value_counts.values():
        prob = count / total_rows
        overall_entropy -= prob * log2(prob)

    # Information gain is the entropy reduction due to the split
    return overall_entropy - attribute_entropy

def build_decision_tree(data, attributes, target_attribute):
    """Recursively builds the decision tree based on the data provided."""
    if not data:
        return DecisionNode(is_leaf=True, decision=None)  # Return a leaf node with no decision for empty data
    
    from collections import Counter
    target_counts = Counter([row[target_attribute] for row in data])

    if len(target_counts) <= 1:
        decision = next(iter(target_counts), None)  # Safely get the single class present, if any
        return DecisionNode(is_leaf=True, decision=decision)

    # If no more attributes left, return a leaf node with the most common target attribute
    if not attributes:
        decision = target_counts.most_common(1)[0][0]
        return DecisionNode(is_leaf=True, decision=decision)

    # Select the best attribute to split on
    best_attribute = choose_best_attribute(data, attributes, target_attribute)
    if not best_attribute:
        decision = target_counts.most_common(1)[0][0]
        return DecisionNode(is_leaf=True, decision=decision)
    
    # Create a new decision node
    node = DecisionNode(attribute=best_attribute)

    # Split data on the best attribute and recurse
    attribute_values = set(row[best_attribute] for row in data)
    for value in attribute_values:
        subset = [row for row in data if row[best_attribute] == value]
        new_attributes = [attr for attr in attributes if attr != best_attribute]
        child = build_decision_tree(subset, new_attributes, target_attribute)
        node.children[value] = child

    return node

def classify(tree, sample):
    """Classify a sample using the decision tree."""
    node = tree
    while not node.is_leaf:
        attribute_value = sample.get(node.attribute)
        if attribute_value not in node.children:
            print(f"Missing branch for attribute '{node.attribute}' with value '{attribute_value}'")
            return None  # Return None if no matching child found
        node = node.children.get(attribute_value)
        if node is None:
            print(f"No child node found for attribute '{node.attribute}' with value '{attribute_value}'")
            return None  # Also handle if no node is found for the attribute
    print(f"Returning decision: {node.decision} for leaf node")
    return node.decision




from math import log2
