package cli

import (
	"bufio"
	"fmt"
	"io"
	"lr3/structures"
)

func Run(input io.Reader, output io.Writer) error {
	c := console{bufio.NewReader(input), output}
	fmt.Fprintln(output, "Выбери структуру:\n1 — массив\n2 — односвязный список\n3 — двусвязный список\n4 — очередь\n5 — стек\n6 — хеш-таблица\n7 — полное двоичное дерево")
	n, err := c.number()
	if err != nil {
		return err
	}
	switch n {
	case 1:
		return c.array()
	case 2:
		var l structures.ForwardList
		return c.list(&l)
	case 3:
		var l structures.List
		return c.list(&l)
	case 4:
		var q structures.Queue[string]
		return c.stackQueue(&q)
	case 5:
		var s structures.Stack
		return c.stackQueue(&s)
	case 6:
		return c.hash()
	case 7:
		return c.tree()
	default:
		return fmt.Errorf("нужен номер от 1 до 7")
	}
}
