package cli

import (
	"fmt"
	"io"
	"lr3/structures"
)

func (c *console) array() error {
	var a structures.Array
	fmt.Fprintln(c.out, "PUSH значение | INSERT индекс значение | GET индекс | SET индекс значение\nDEL индекс | SIZE | EMPTY | PRINT | EXIT")
	for {
		command, err := c.word()
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		switch command {
		case "EXIT":
			return nil
		case "PUSH":
			v, err := c.word()
			if err != nil {
				return err
			}
			a.PushBack(v)
		case "INSERT", "SET":
			i, err := c.index()
			if err != nil {
				return err
			}
			v, err := c.word()
			if err != nil {
				return err
			}
			if command == "INSERT" {
				err = a.PushIndex(v, i)
			} else {
				err = a.Set(i, v)
			}
			if err != nil {
				return err
			}
		case "GET":
			i, err := c.index()
			if err != nil {
				return err
			}
			v, err := a.Get(i)
			if err != nil {
				return err
			}
			fmt.Fprintln(c.out, v)
			continue
		case "DEL":
			i, err := c.index()
			if err != nil {
				return err
			}
			a.PopIndex(i)
		case "SIZE":
			fmt.Fprintln(c.out, a.Length())
			continue
		case "EMPTY":
			c.printBool(a.Empty())
			continue
		case "PRINT":
		default:
			return fmt.Errorf("неизвестная команда массива")
		}
		a.Print(c.out)
	}
}
