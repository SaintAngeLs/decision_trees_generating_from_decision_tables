from math import log2
from collections import Counter

class DecisionNode:
    """A node in the decision tree, which may be either a decision node or a leaf node."""
    def __init__(self, attribute=None, threshold=None, is_leaf=False, decision=None):
        self.attribute = attribute  # Attribute to split on at this node
        self.threshold = threshold  # Threshold value for numerical splits
        self.is_leaf = is_leaf      # Whether this is a leaf node
        self.decision = decision    # Decision if this is a leaf
        self.children = {}          # Dictionary to hold children nodes: attribute_value or 'left'/'right' for numerical splits

def calculate_entropy(subset, target_attribute):
    """Calculates entropy for a subset of data."""
    if not subset:
        return 0
    label_counts = Counter(row[target_attribute] for row in subset)
    total = len(subset)
    entropy = 0
    for count in label_counts.values():
        probability = count / total
        entropy -= probability * log2(probability)
    return entropy

def find_best_split(data, attribute, target_attribute):
    max_gain = 0  # Initialize max_gain at the start to ensure it has a default value
    best_split = None
    try:
        float(data[0][attribute])  # Check if attribute is numeric
        numerical = True
    except ValueError:
        numerical = False

    if numerical:
        unique_values = sorted(set(row[attribute] for row in data))
        for i in range(1, len(unique_values)):
            split_value = (unique_values[i - 1] + unique_values[i]) / 2
            left_subset = [row for row in data if row[attribute] <= split_value]
            right_subset = [row for row in data if row[attribute] > split_value]
            gain = information_gain_for_split(data, left_subset, right_subset, target_attribute)
            if gain > max_gain:
                max_gain = gain
                best_split = split_value
    else:
        for value in set(row[attribute] for row in data):
            subset = [row for row in data if row[attribute] == value]
            gain = calculate_entropy(data, target_attribute) - calculate_entropy(subset, target_attribute)
            if gain > max_gain:
                max_gain = gain
                best_split = value

    return best_split, max_gain, numerical




def information_gain_for_split(data, left, right, target_attribute):
    """Calculate information gain from a particular binary split."""
    total_len = len(data)
    left_prob = len(left) / total_len
    right_prob = len(right) / total_len
    return calculate_entropy(data, target_attribute) - (
        left_prob * calculate_entropy(left, target_attribute) + right_prob * calculate_entropy(right, target_attribute))

def choose_best_attribute(data, attributes, target_attribute):
    best_attribute = None
    best_threshold = None
    max_gain = 0
    numerical_split = False
    
    for attribute in attributes:
        split, gain, is_numerical = find_best_split(data, attribute, target_attribute)
        if gain > max_gain:
            max_gain = gain
            best_attribute = attribute
            best_threshold = split if is_numerical else None
            numerical_split = is_numerical
    
    return best_attribute, best_threshold, numerical_split

def build_decision_tree(data, attributes, target_attribute):
    """Recursively builds the decision tree based on the data provided."""
    if not data:
        return DecisionNode(is_leaf=True, decision=None)  # Return a leaf node with no decision for empty data
    
    target_counts = Counter([row[target_attribute] for row in data])
    if len(target_counts) == 1:
        decision = next(iter(target_counts))
        return DecisionNode(is_leaf=True, decision=decision)

    if not attributes:
        decision = target_counts.most_common(1)[0][0]
        return DecisionNode(is_leaf=True, decision=decision)

    best_attribute, best_threshold, numerical_split = choose_best_attribute(data, attributes, target_attribute)
    if not best_attribute:
        decision = target_counts.most_common(1)[0][0]
        return DecisionNode(is_leaf=True, decision=decision)

    node = DecisionNode(attribute=best_attribute, threshold=best_threshold)
    if numerical_split:
        left_data = [row for row in data if row[best_attribute] <= best_threshold]
        right_data = [row for row in data if row[best_attribute] > best_threshold]
        node.children['left'] = build_decision_tree(left_data, attributes, target_attribute)
        node.children['right'] = build_decision_tree(right_data, attributes, target_attribute)
    else:
        attribute_values = set(row[best_attribute] for row in data)
        for value in attribute_values:
            subset = [row for row in data if row[best_attribute] == value]
            node.children[value] = build_decision_tree(subset, attributes, target_attribute)

    return node

def classify(tree, sample):
    """Classify a sample using the decision tree."""
    node = tree
    while not node.is_leaf:
        if node.threshold is not None:  # Numerical attribute
            attribute_value = sample[node.attribute]
            branch = 'left' if attribute_value <= node.threshold else 'right'
        else:  # Categorical attribute
            attribute_value = sample[node.attribute]
            branch = attribute_value
        
        if branch not in node.children:
            print(f"Missing branch for attribute '{node.attribute}' with value '{attribute_value}'")
            return None  # Handle missing branches
        node = node.children[branch]
    
    print(f"Returning decision: {node.decision} for leaf node")
    return node.decision
