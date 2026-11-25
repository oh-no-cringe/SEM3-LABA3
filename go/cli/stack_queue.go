package cli

import (
	"fmt"
	"io"
)

type stackQueueMethods interface {
	Push(string)
	Pop() (string, error)
	Empty() bool
	Print(io.Writer)
}

func (c *console) stackQueue(data stackQueueMethods, kind string) error {
	fmt.Fprintln(c.out, "PUSH значение | POP | EMPTY | PRINT | EXIT")
	for {
		command, err := c.word()
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		if command == "SAVE" || command == "LOAD" {
			if err := c.file(command, data, kind); err != nil {
				return err
			}
			continue
		}
		switch command {
		case "EXIT":
			return nil
		case "PUSH":
			v, err := c.word()
			if err != nil {
				return err
			}
			data.Push(v)
		case "POP":
			if data.Empty() {
				fmt.Fprintln(c.out, "EMPTY")
			} else {
				v, err := data.Pop()
				if err != nil {
					return err
				}
				fmt.Fprintln(c.out, v)
			}
			continue
		case "EMPTY":
			c.printBool(data.Empty())
			continue
		case "PRINT":
		default:
			return fmt.Errorf("неизвестная команда стека или очереди")
		}
		data.Print(c.out)
	}
}
