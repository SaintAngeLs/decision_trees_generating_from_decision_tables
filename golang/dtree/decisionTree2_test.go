package dtree

import (
	"golang/utils/imp"
	"os"
	"path/filepath"
	"testing"
)

func TestTreeStructure2(t *testing.T) {
	path, _ := os.Getwd()
	parentPath := filepath.Dir(path)
	data := imp.ReadCsvFile(parentPath + "/data/data2.csv")
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
