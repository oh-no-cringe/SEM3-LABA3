package structures

import "fmt"

type Array struct {
	data []string
	size int
}

func (a *Array) reserve() {
	if a.size < len(a.data) {
		return
	}
	capacity := len(a.data) * 2
	if capacity == 0 {
		capacity = 4
	}
	next := make([]string, capacity)
	copy(next, a.data)
	a.data = next
}
func (a *Array) PushBack(value string) {
	a.reserve()
	a.data[a.size] = value
	a.size++
}
func (a *Array) PushIndex(value string, index int) error {
	if index < 0 || index > a.size {
		return fmt.Errorf("array index out of range")
	}
	a.reserve()
	copy(a.data[index+1:a.size+1], a.data[index:a.size])
	a.data[index] = value
	a.size++
	return nil
}
func (a *Array) Get(index int) (string, error) {
	if index < 0 || index >= a.size {
		return "", fmt.Errorf("array index out of range")
	}
	return a.data[index], nil
}
func (a *Array) Set(index int, value string) error {
	if index < 0 || index >= a.size {
		return fmt.Errorf("array index out of range")
	}
	a.data[index] = value
	return nil
}
func (a *Array) PopIndex(index int) {
	if index < 0 || index >= a.size {
		return
	}
	copy(a.data[index:a.size-1], a.data[index+1:a.size])
	a.size--
	a.data[a.size] = ""
}
func (a *Array) Length() int {
	return a.size
}
func (a *Array) Empty() bool {
	return a.size == 0
}
func (a *Array) Values() []string {
	return append([]string{}, a.data[:a.size]...)
}

func (a *Array) Clear() {
	for i := 0; i < a.size; i++ {
		a.data[i] = ""
	}
	a.size = 0
}

func (a *Array) Swap(other *Array) {
	a.data, other.data = other.data, a.data
	a.size, other.size = other.size, a.size
}
