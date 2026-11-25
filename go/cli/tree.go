package cli

import (
	"fmt"
	"io"
	"lr3/structures"
)

func (c *console) tree() error {
	var tree structures.Tree
	fmt.Fprintln(c.out, "INSERT число | FIND число | CHECK | PRE | IN | POST | EMPTY | PRINT | EXIT")
	for {
		command, err := c.word()
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		if command == "SAVE" || command == "LOAD" {
			if err := c.file(command, &tree, "tree"); err != nil {
				return err
			}
			continue
		}
		switch command {
		case "EXIT":
			return nil
		case "INSERT", "FIND":
			v, err := c.number()
			if err != nil {
				return err
			}
			if command == "INSERT" {
				tree.Insert(v)
				tree.Print(c.out)
			} else {
				c.printBool(tree.Search(v))
			}
		case "CHECK":
			c.printBool(tree.IsComplete())
		case "PRE", "IN", "POST":
			tree.PrintOrder(c.out, command)
		case "EMPTY":
			c.printBool(tree.Empty())
		case "PRINT":
			tree.Print(c.out)
		default:
			return fmt.Errorf("неизвестная команда дерева")
		}
	}
}
