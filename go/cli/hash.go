package cli

import (
	"fmt"
	"io"
	"lr3/structures"
)

func (c *console) hash() error {
	fmt.Fprintln(c.out, "Выбери метод: 1 — линейное пробирование, 2 — двойное хеширование")
	method, err := c.number()
	if err != nil {
		return err
	}
	if method != 1 && method != 2 {
		return fmt.Errorf("нужен метод 1 или 2")
	}
	table := structures.NewHash(method == 2)
	fmt.Fprintln(c.out, "SET ключ значение | GET ключ | HAS ключ | DEL ключ | PRINT | EXIT")
	for {
		command, err := c.word()
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		if command == "SAVE" || command == "LOAD" {
			if err := c.file(command, table, "hash"); err != nil {
				return err
			}
			continue
		}
		if command == "EXIT" {
			return nil
		}
		if command == "PRINT" {
			table.Print(c.out)
			continue
		}
		if command != "SET" && command != "GET" && command != "HAS" && command != "DEL" {
			return fmt.Errorf("неизвестная команда хеш-таблицы")
		}
		key, err := c.word()
		if err != nil {
			return err
		}
		switch command {
		case "SET":
			value, err := c.word()
			if err != nil {
				return err
			}
			table.Set(key, value)
		case "GET":
			value, ok := table.Get(key)
			if ok {
				fmt.Fprintln(c.out, value)
			} else {
				fmt.Fprintln(c.out, "NOT_FOUND")
			}
			continue
		case "HAS":
			c.printBool(table.Has(key))
			continue
		case "DEL":
			table.Del(key)
		}
		fmt.Fprintln(c.out, "OK")
	}
}
