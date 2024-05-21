package dtree

import (
	"golang/utils/imp"
	"os"
	"path/filepath"
	"testing"
)

func TestTreeStructure3(t *testing.T) {
	path, _ := os.Getwd()
	parentPath := filepath.Dir(path)
	data := imp.ReadCsvFile(parentPath + "/data/data3.csv")
	attributes := []string{"name", "age", "gender", "minor", "children", "salary"}
	targetAttribute := "maritalStatus"
	tree := BuildDecisionTree(data, attributes, targetAttribute)
	if tree.Attribute == "" {
		t.Errorf("Expected tree to have an attribute")
	}
}

func TestEmptyData3(t *testing.T) {
	data := make([]map[string]string, 0)
	attributes := []string{"name", "age", "gender", "minor", "children", "salary"}
	targetAttribute := "maritalStatus"
	tree := BuildDecisionTree(data, attributes, targetAttribute)
	if !tree.IsLeaf || tree.Decision != "" {
		t.Errorf("Expected tree to be empty")
	}
}
