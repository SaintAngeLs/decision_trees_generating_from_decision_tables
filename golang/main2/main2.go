package main

import (
	"golang/dtree"
	"golang/utils/exp"
	"golang/utils/imp"
	"os"
)

func main() {
	path, _ := os.Getwd()
	data := imp.ReadCsvFile(path + "/data/data3.csv")
	attributes := []string{"age", "gender", "maritalStatus", "minor", "children"}
	targetAttribute := "salary"

	tree := dtree.BuildDecisionTree(data, attributes, targetAttribute)

	err := exp.ExportDecisionTreeToJson(tree, "simple_decision_tree.json")
	if err != nil {
		panic(err)
	}
}
