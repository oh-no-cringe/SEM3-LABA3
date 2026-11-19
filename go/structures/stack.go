package structures

import "fmt"

type Stack struct{ head *forwardNode }

func (s *Stack) Push(v string) {
	s.head = &forwardNode{v, s.head}
}
func (s *Stack) Pop() (string, error) {
	if s.head == nil {
		return "", fmt.Errorf("stack is empty")
	}
	v := s.head.value
	s.head = s.head.next
	return v, nil
}
func (s *Stack) Empty() bool {
	return s.head == nil
}
func (s *Stack) Values() []string {
	v := []string{}
	for c := s.head; c != nil; c = c.next {
		v = append(v, c.value)
	}
	return v
}
