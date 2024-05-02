package dtree

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
