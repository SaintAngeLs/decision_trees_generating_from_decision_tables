package dtree

import "math"

type DecisionNode struct {
	Attribute string
	IsLeaf    bool
	Decision  string
	Children  []DecisionNode
}

func New(attribute string, isLeaf bool, decision string) DecisionNode {
	return DecisionNode{Attribute: attribute,
		IsLeaf:   isLeaf,
		Decision: decision,
		Children: make([]DecisionNode, 0),
	}
}

func chooseBestAttribute(data []map[string]string, attributes []string, targetAttribute string) string {
	bestAttribute := ""
	maxGain := 0.0

	for _, attribute := range attributes {
		gain := informationGain(data, attribute, targetAttribute)
		if gain > maxGain {
			maxGain = gain
			bestAttribute = attribute
		}
	}
	return bestAttribute
}

func informationGain(data []map[string]string, attribute string, targetAttribute string) float64 {
	valueCounts := make(map[string]map[string]int)
	// Calculate the frequency of each value in the target attribute
	for _, row := range data {
		value := row[attribute]
		targetValue := row[targetAttribute]
		valueCounts[value][targetValue] += 1
	}

	// Calculate the entropy of each subset of records weighted by their probability.
	totalRows := len(data)
	attributeEntropy := 0.0

	for _, value := range valueCounts {
		valueEntropy := 0.0
		totalValueRows := 0
		for _, val := range value {
			totalValueRows += val
		}

		for _, outcomeCount := range value {
			probability := float64(outcomeCount / totalValueRows)
			valueEntropy -= probability * math.Log2(probability)
		}
		attributeEntropy += float64(totalValueRows/totalRows) * valueEntropy
	}

	// Calculate the entropy before the split

	overallEntropy := 0.0
	targetValueCounts := make(map[string]int)

	for _, row := range data {
		targetValue := row[targetAttribute]
		targetValueCounts[targetValue]++
	}
	for _, count := range targetValueCounts {
		probability := float64(count / totalRows)
		overallEntropy -= probability * math.Log2(probability)
	}

	return overallEntropy - attributeEntropy
}

func BuildDecisionTree(data []map[string]string, attributes []string, targetAttribute string) DecisionNode {
	if len(data) == 0 {
		return New("", true, "")
	}

	targetCounts := make(map[string]int)

	for _, row := range data {
		targetCounts[row[targetAttribute]]++
	}

	if len(targetCounts) <= 1 {
		var decision string
		for s := range targetCounts {
			decision = s
		}
		return New("", true, decision)
	}

	// If no more attributes left, return a leaf node with the most common target attribute
	if len(attributes) == 0 {
		var decision string
		count := 0
		for s, i := range targetCounts {
			if count < i {
				count = i
				decision = s
			}
		}
		return New("", true, decision)
	}

	// Select the best attribute to split on
	bestAttribute := chooseBestAttribute(data, attributes, targetAttribute)
	if bestAttribute == "" {
		var decision string
		count := 0
		for s, i := range targetCounts {
			if count < i {
				count = i
				decision = s
			}
		}
		return New("", true, decision)
	}
	// Create a new decision node
	node := New(bestAttribute, false, "")

	// Split data on the best attribute and recurse

	return node
}
