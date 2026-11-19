package structures

import "strconv"

type treeNode struct {
	value int
	left  *treeNode
	right *treeNode
}

type Tree struct {
	root *treeNode
}

func (t *Tree) Empty() bool {
	return t.root == nil
}

func (t *Tree) Insert(value int) {
	newNode := &treeNode{value: value}
	if t.root == nil {
		t.root = newNode
		return
	}
	var next Queue[*treeNode]
	next.Push(t.root)
	for !next.Empty() {
		node, _ := next.Pop()
		if node.left == nil {
			node.left = newNode
			return
		}
		if node.right == nil {
			node.right = newNode
			return
		}
		next.Push(node.left)
		next.Push(node.right)
	}
}

func findValue(node *treeNode, value int) bool {
	if node == nil {
		return false
	}
	if node.value == value {
		return true
	}
	return findValue(node.left, value) || findValue(node.right, value)
}

func (t *Tree) Search(value int) bool {
	return findValue(t.root, value)
}

func (t *Tree) IsComplete() bool {
	var next Queue[*treeNode]
	next.Push(t.root)
	gap := false
	for !next.Empty() {
		node, _ := next.Pop()
		if node == nil {
			gap = true
		} else {
			if gap {
				return false
			}
			next.Push(node.left)
			next.Push(node.right)
		}
	}
	return true
}

func (t *Tree) Values() []string {
	values := []string{}
	var next Queue[*treeNode]
	if t.root != nil {
		next.Push(t.root)
	}
	for !next.Empty() {
		node, _ := next.Pop()
		values = append(values, strconv.Itoa(node.value))
		if node.left != nil {
			next.Push(node.left)
		}
		if node.right != nil {
			next.Push(node.right)
		}
	}
	return values
}

func visit(node *treeNode, order string, values *[]string) {
	if node == nil {
		return
	}
	if order == "PRE" {
		*values = append(*values, strconv.Itoa(node.value))
	}
	visit(node.left, order, values)
	if order == "IN" {
		*values = append(*values, strconv.Itoa(node.value))
	}
	visit(node.right, order, values)
	if order == "POST" {
		*values = append(*values, strconv.Itoa(node.value))
	}
}

func (t *Tree) Traverse(order string) []string {
	values := []string{}
	visit(t.root, order, &values)
	return values
}
