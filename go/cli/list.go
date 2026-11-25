package cli

import (
	"fmt"
	"io"
)

type listMethods interface {
	PushBack(string)
	PushFront(string)
	PushIndex(string, int)
	PopIndex(int)
	PopFront()
	PopBack()
	EraseValue(string) bool
	Find(string) bool
	Empty() bool
	Print(io.Writer)
}

func (c *console) list(l listMethods) error {
	fmt.Fprintln(c.out, "PUSH значение | FRONT значение | INSERT индекс значение | DEL индекс\nDEL_VALUE значение | FIND значение | POP_FRONT | POP_BACK | EMPTY | PRINT | EXIT")
	for {
		command, err := c.word()
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		if command == "SAVE" || command == "LOAD" {
			if err := c.file(command, l, "list"); err != nil {
				return err
			}
			continue
		}
		switch command {
		case "EXIT":
			return nil
		case "PUSH", "FRONT", "DEL_VALUE", "FIND":
			v, err := c.word()
			if err != nil {
				return err
			}
			switch command {
			case "PUSH":
				l.PushBack(v)
			case "FRONT":
				l.PushFront(v)
			case "DEL_VALUE":
				l.EraseValue(v)
			case "FIND":
				c.printBool(l.Find(v))
				continue
			}
		case "INSERT":
			i, err := c.index()
			if err != nil {
				return err
			}
			v, err := c.word()
			if err != nil {
				return err
			}
			l.PushIndex(v, i)
		case "DEL":
			i, err := c.index()
			if err != nil {
				return err
			}
			l.PopIndex(i)
		case "POP_FRONT":
			l.PopFront()
		case "POP_BACK":
			l.PopBack()
		case "EMPTY":
			c.printBool(l.Empty())
			continue
		case "PRINT":
		default:
			return fmt.Errorf("неизвестная команда списка")
		}
		l.Print(c.out)
	}
}
