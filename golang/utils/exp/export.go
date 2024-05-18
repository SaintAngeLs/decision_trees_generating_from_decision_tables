package exp

import (
	"encoding/json"
	"golang/dtree"
	"os"
)

func ExportDecisionTreeToJson(tree dtree.DecisionNode, filename string) error {
	jsonTree, err := json.Marshal(tree)
	if err != nil {
		return err
	}
	err = os.WriteFile("output/json/"+filename, jsonTree, 0644)
	return err
}
