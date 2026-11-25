package cli

import (
	"fmt"
	"lr3/structures"
)

func (c *console) file(command string, object any, kind string) error {
	format, err := c.word()
	if err != nil {
		return err
	}
	path, err := c.word()
	if err != nil {
		return err
	}
	if format != "text" && format != "binary" {
		return fmt.Errorf("формат: text или binary")
	}
	if command == "SAVE" {
		err = structures.SaveFile(object, path, kind, format == "binary")
	} else {
		err = structures.LoadFile(object, path, kind, format == "binary")
	}
	if err == nil {
		fmt.Fprintln(c.out, "OK")
	}
	return err
}
