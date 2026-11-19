package structures

import "fmt"

type queueNode[T any] struct {
	value T
	next  *queueNode[T]
}

type Queue[T any] struct {
	head *queueNode[T]
	tail *queueNode[T]
}

func (q *Queue[T]) Push(value T) {
	newNode := &queueNode[T]{value: value}
	if q.tail == nil {
		q.head = newNode
	} else {
		q.tail.next = newNode
	}
	q.tail = newNode
}

func (q *Queue[T]) Pop() (T, error) {
	if q.head == nil {
		var value T
		return value, fmt.Errorf("очередь пуста")
	}
	temp := q.head
	q.head = temp.next
	if q.head == nil {
		q.tail = nil
	}
	return temp.value, nil
}

func (q *Queue[T]) Empty() bool {
	return q.head == nil
}

func (q *Queue[T]) Values() []T {
	values := []T{}
	for node := q.head; node != nil; node = node.next {
		values = append(values, node.value)
	}
	return values
}
