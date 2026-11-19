package cli

import (
	"bufio"
	"fmt"
	"io"
	"strconv"
	"strings"
)

type console struct {
	in  *bufio.Reader
	out io.Writer
}

func space(c byte) bool { return strings.ContainsRune(" \t\r\n\v\f", rune(c)) }
func (c *console) word() (string, error) {
	var first byte
	for {
		b, err := c.in.ReadByte()
		if err != nil {
			return "", err
		}
		if !space(b) {
			first = b
			break
		}
	}
	var text strings.Builder
	if first == '"' {
		for {
			b, err := c.in.ReadByte()
			if err != nil {
				return "", fmt.Errorf("незакрытая строка")
			}
			if b == '"' {
				next, err := c.in.Peek(1)
				if err == nil && !space(next[0]) {
					return "", fmt.Errorf("между аргументами нужен пробел")
				}
				return text.String(), nil
			}
			if b == '\\' {
				b, err = c.in.ReadByte()
				if err != nil {
					return "", fmt.Errorf("незакрытая строка")
				}
			}
			text.WriteByte(b)
		}
	}
	text.WriteByte(first)
	for {
		b, err := c.in.ReadByte()
		if err == io.EOF {
			return text.String(), nil
		}
		if err != nil {
			return "", err
		}
		if space(b) {
			return text.String(), nil
		}
		text.WriteByte(b)
	}
}
func (c *console) number() (int, error) {
	s, err := c.word()
	if err != nil {
		return 0, err
	}
	n, err := strconv.ParseInt(s, 10, 32)
	if err != nil {
		return 0, fmt.Errorf("нужно целое число")
	}
	return int(n), nil
}
func (c *console) index() (int, error) {
	n, err := c.number()
	if err != nil {
		return 0, err
	}
	if n < 0 {
		return 0, fmt.Errorf("индекс должен быть неотрицательным")
	}
	return n, nil
}
func (c *console) printBool(b bool) {
	if b {
		fmt.Fprintln(c.out, "TRUE")
	} else {
		fmt.Fprintln(c.out, "FALSE")
	}
}
