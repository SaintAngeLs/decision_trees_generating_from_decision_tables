import pytest
from decision_tree import build_decision_tree, classify, DecisionNode
import pandas as pd
from io import StringIO

# Sample data as a string to mimic reading from a file
DATA = """\
age,income,student,buys_computer
youth,high,no,no
youth,high,no,yes
middle_aged,medium,no,yes
senior,medium,yes,yes
senior,low,no,no
middle_aged,low,no,yes
youth,medium,yes,yes
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
    attributes = ['age', 'income', 'student']
    target_attribute = 'buys_computer'
    tree = build_decision_tree(data, attributes, target_attribute)
    return tree

# Test the tree building
def test_tree_structure(decision_tree):
    assert isinstance(decision_tree, DecisionNode), "The tree root should be a DecisionNode"
    assert decision_tree.attribute is not None, "The root node should have an attribute to split on"

# Test classification correctness
@pytest.mark.parametrize("sample, expected", [
    ({'age': 'youth', 'income': 'high', 'student': 'no'}, 'no'),
    ({'age': 'middle_aged', 'income': 'low', 'student': 'no'}, 'yes'),
    ({'age': 'senior', 'income': 'medium', 'student': 'yes'}, 'yes'),
    ({'age': 'senior', 'income': 'high', 'student': 'no'},None)  # edge case not in data
])
def test_classification(decision_tree, sample, expected):
    decision = classify(decision_tree, sample)
    assert decision == expected, f"Expected decision for {sample} to be {expected}, but got {decision}"

# This tests an edge case where data is empty
def test_empty_data():
    data = []
    attributes = ['age', 'income', 'student']
    target_attribute = 'buys_computer'
    tree = build_decision_tree(data, attributes, target_attribute)
    assert tree.is_leaf and tree.decision is None, "Tree should be a leaf with no decision on empty data"
