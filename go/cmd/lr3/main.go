package main

import (
	"fmt"
	"io"
	"lr3/cli"
	"os"
)

func run(input io.Reader, output, errors io.Writer) int {
	if err := cli.Run(input, output); err != nil {
		fmt.Fprintln(errors, "Ошибка:", err)
		return 1
	}
	return 0
}
func main() { os.Exit(run(os.Stdin, os.Stdout, os.Stderr)) }
