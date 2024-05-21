package main

import (
	"fmt"
	"golang/dtree"
	"golang/utils/exp"
	"golang/utils/imp"
	"os"
)

func main() {
	path, _ := os.Getwd()
	data := imp.ReadCsvFile(path + "/data/data1.csv")
	attributes := []string{"age", "income", "student", "credit_rating"}
	targetAttribute := "buys_computer"

	tree := dtree.BuildDecisionTree(data, attributes, targetAttribute)

	err := exp.ExportDecisionTreeToJson(tree, "comp_decision_tree.json")
	if err != nil {
		panic(err)
	}

	sample := make(map[string]string)
	sample["age"] = "youth"
	sample["income"] = "high"
	sample["student"] = "no"
	decision, err := dtree.Classify(tree, sample)
	if err != nil {
		panic(err)
	}
	fmt.Println("Decision: ", decision)
}
