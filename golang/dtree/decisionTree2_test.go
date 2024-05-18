package dtree

import (
	"golang/utils"
	"testing"
)

func TestTreeStructure2(t *testing.T) {
	data := utils.ReadCsvFile("/Users/dima/mini_ml/golang/data/data2.csv")
	attributes := []string{"age", "income", "student", "credit_rating"}
	targetAttribute := "buys_computer"
	tree := BuildDecisionTree(data, attributes, targetAttribute)
	if tree.Attribute == "" {
		t.Errorf("Expected tree to have an attribute")
	}
}

func TestEmptyData2(t *testing.T) {
	data := make([]map[string]string, 0)
	attributes := []string{"age", "income", "student", "credit_rating"}
	targetAttribute := "buys_computer"
	tree := BuildDecisionTree(data, attributes, targetAttribute)
	if !tree.IsLeaf || tree.Decision != "" {
		t.Errorf("Expected tree to be empty")
	}
}
