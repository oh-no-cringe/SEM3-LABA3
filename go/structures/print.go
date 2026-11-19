package structures

import (
	"fmt"
	"io"
	"strings"
)

func printValues(output io.Writer, values []string) {
	fmt.Fprintln(output, "["+strings.Join(values, ", ")+"]")
}

func (a *Array) Print(output io.Writer) {
	printValues(output, a.Values())
}
func (l *ForwardList) Print(output io.Writer) {
	printValues(output, l.Values())
}
func (l *List) Print(output io.Writer) {
	printValues(output, l.Values())
}
func (s *Stack) Print(output io.Writer) {
	printValues(output, s.Values())
}
func (q *Queue[T]) Print(output io.Writer) {
	fmt.Fprint(output, "[")
	for node := q.head; node != nil; node = node.next {
		if node != q.head {
			fmt.Fprint(output, ", ")
		}
		fmt.Fprint(output, node.value)
	}
	fmt.Fprintln(output, "]")
}
func (t *Tree) Print(output io.Writer) {
	printValues(output, t.Values())
}
func (t *Tree) PrintOrder(output io.Writer, order string) {
	printValues(output, t.Traverse(order))
}
func (t *HashTable) Print(output io.Writer) {
	fmt.Fprint(output, "[")
	for i, key := range t.Names() {
		if i > 0 {
			fmt.Fprint(output, ", ")
		}
		value, _ := t.Get(key)
		fmt.Fprintf(output, "%q: %q", key, value)
	}
	fmt.Fprintln(output, "]")
}
