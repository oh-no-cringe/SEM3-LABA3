package structures

import (
	"strconv"
	"testing"
)

// Один вызов — создание и заполнение структуры из 1000 элементов.
// ns/op в выводе Go относится ко всему этому пакету операций.
func BenchmarkArray(b *testing.B) {
	for i := 0; i < b.N; i++ {
		var a Array
		for j := 0; j < 1000; j++ {
			a.PushBack("value")
		}
	}
}
func BenchmarkForwardList(b *testing.B) {
	for i := 0; i < b.N; i++ {
		var a ForwardList
		for j := 0; j < 1000; j++ {
			a.PushBack("value")
		}
	}
}
func BenchmarkList(b *testing.B) {
	for i := 0; i < b.N; i++ {
		var a List
		for j := 0; j < 1000; j++ {
			a.PushBack("value")
		}
	}
}
func BenchmarkQueue(b *testing.B) {
	for i := 0; i < b.N; i++ {
		var a Queue[string]
		for j := 0; j < 1000; j++ {
			a.Push("value")
		}
	}
}
func BenchmarkStack(b *testing.B) {
	for i := 0; i < b.N; i++ {
		var a Stack
		for j := 0; j < 1000; j++ {
			a.Push("value")
		}
	}
}
func BenchmarkHashTable(b *testing.B) {
	for i := 0; i < b.N; i++ {
		a := NewHash(false)
		for j := 0; j < 1000; j++ {
			a.Set(strconv.Itoa(j), "value")
		}
	}
}
func BenchmarkTree(b *testing.B) {
	for i := 0; i < b.N; i++ {
		var a Tree
		for j := 0; j < 1000; j++ {
			a.Insert(j)
		}
	}
}
