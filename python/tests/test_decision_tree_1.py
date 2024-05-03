import pytest
from decision_tree import build_decision_tree, classify, DecisionNode
import pandas as pd
from io import StringIO

# Enhanced sample data with additional attributes and complexity
DATA = """\
age,income,student,credit_rating,buys_computer
youth,high,no,fair,no
youth,high,no,excellent,no
middle_aged,medium,no,fair,yes
senior,medium,yes,fair,yes
senior,low,no,fair,no
middle_aged,low,no,excellent,yes
youth,medium,yes,fair,yes
middle_aged,high,yes,fair,yes
senior,medium,no,excellent,no
"""

# Function to load data from a string
def load_data(data_string):
    data_file = StringIO(data_string)
    data = pd.read_csv(data_file)
    return data.to_dict('records')

# Fixture to build the tree from the sample data
@pytest.fixture
def decision_tree():
    data = load_data(DATA)
    attributes = ['age', 'income', 'student', 'credit_rating']
    target_attribute = 'buys_computer'
    tree = build_decision_tree(data, attributes, target_attribute)
    return tree

# Test the tree structure and its ability to correctly assign an attribute
def test_tree_structure(decision_tree):
    assert isinstance(decision_tree, DecisionNode), "The tree root should be a DecisionNode"
    assert decision_tree.attribute is not None, "The root node should have an attribute to split on"

# Test classification correctness with more complex scenarios
@pytest.mark.parametrize("sample, expected", [
    ({'age': 'youth', 'income': 'high', 'student': 'no', 'credit_rating': 'fair'}, 'no'),
    ({'age': 'middle_aged', 'income': 'medium', 'student': 'no', 'credit_rating': 'fair'}, 'yes'),
    ({'age': 'senior', 'income': 'medium', 'student': 'yes', 'credit_rating': 'fair'}, 'yes'),
    ({'age': 'middle_aged', 'income': 'high', 'student': 'yes', 'credit_rating': 'fair'}, 'yes'),
    ({'age': 'senior', 'income': 'medium', 'student': 'no', 'credit_rating': 'excellent'}, 'no'),
    ({'age': 'youth', 'income': 'low', 'student': 'yes', 'credit_rating': 'fair'}, None)  # Example of a not covered case
])
def test_classification(decision_tree, sample, expected):
    decision = classify(decision_tree, sample)
    assert decision == expected, f"Expected decision for {sample} to be {expected}, but got {decision}"

# This test checks the behavior with an entirely empty data set
def test_empty_data():
    data = []
    attributes = ['age', 'income', 'student', 'credit_rating']
    target_attribute = 'buys_computer'
    tree = build_decision_tree(data, attributes, target_attribute)
    assert tree.is_leaf and tree.decision is None, "Tree should be a leaf with no decision on empty data"
