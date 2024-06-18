Here's a README.md crafted for an advanced machine learning project that specifically includes mathematical formulations relevant to the generation of decision trees from decision tables:

# Decision Tree Generator from Decision Tables

## Introduction

This project automates the conversion of decision tables into decision trees, integrating classical decision-making theories with machine learning algorithms. This tool is crucial for simplifying complex decision processes in various domains such as healthcare, finance, and more.

## Theoretical Background

### Decision Tables

A decision table is a structured way to represent conditional logic by listing scenarios and their respective actions. Mathematically, a decision table `T` can be expressed as:

- **Scenarios (S)**: Possible states or inputs represented as rows.
- **Conditions (C)**: Criteria based on which decisions are made, represented as columns.
- **Actions (A)**: Outcomes or operations to perform when conditions are satisfied.

### Decision Trees

A decision tree is a flowchart-like structure in which each internal node represents a "test" on an attribute, each branch represents the outcome of the test, and each leaf node represents a class label (decision). The paths from root to leaf represent classification rules.

### Information Entropy

The entropy `H(S)` of a set `S` is defined as:

$$
H(S) = -\sum_{i=1}^n p_i \log_2 p_i
$$



Where `p_i` is the proportion (frequency) of the number of elements in class `i` to the number of elements in set `S`.

### Information Gain

Used to decide the best feature that gets to go on the node of the tree. It calculates the difference between entropy before split and average entropy after the split of the dataset based on given attribute values.


$$
\text{Gain}(S, A) = H(S) - \sum \left(\frac{|S_v|}{|S|} \cdot H(S_v)\right)
$$


Where:
- `S_v` is the subset of `S` for each value `v` of attribute `A`.
- `|S_v|` and `|S|` are the number of elements in `S_v` and `S`, respectively.

## Installation

To set up the project environment:

```bash
git clone https://github.com/yourusername/decision-tree-generator.git
cd decision-tree-generator
pip install -r requirements.txt
```

## Usage

Run the following command to generate a decision tree from a CSV file containing the decision table:

```bash
python decision_tree_generator.py --input your_decision_table.csv
```

## Contributing

Contributors are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.

## Authors

- **Dmytro Tolstoi**
- **Oleh Kiprik**
- **Andrii Voznesenskyi**
