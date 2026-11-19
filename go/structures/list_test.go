package structures

import (
	"bytes"
	"github.com/stretchr/testify/require"
	"io"
	"testing"
)

type testList interface {
	PushFront(string)
	PushBack(string)
	PushIndex(string, int)
	PopFront()
	PopBack()
	PopIndex(int)
	InsertBefore(string, string) bool
	InsertAfter(string, string) bool
	EraseBefore(string) bool
	EraseAfter(string) bool
	EraseValue(string) bool
	Find(string) bool
	Empty() bool
	Values() []string
	Print(io.Writer)
}

func checkList(t *testing.T, newList func() testList) {
	t.Helper()
	t.Run("empty", func(t *testing.T) {
		l := newList()
		require.True(t, l.Empty())
		l.PopFront()
		l.PopBack()
		l.PopIndex(9)
		l.PopIndex(-1)
		l.PushIndex("x", -1)
		require.False(t, l.Find("x"))
		require.False(t, l.EraseValue("x"))
		require.False(t, l.InsertBefore("x", "y"))
		require.False(t, l.InsertAfter("x", "y"))
		require.False(t, l.EraseBefore("x"))
		require.False(t, l.EraseAfter("x"))
		require.Empty(t, l.Values())
		var b bytes.Buffer
		l.Print(&b)
		require.Equal(t, "[]\n", b.String())
	})
	t.Run("order", func(t *testing.T) {
		l := newList()
		l.PushBack("a")
		l.PushBack("b")
		l.PushFront("z")
		l.PushIndex("x", 2)
		l.PushIndex("first", 0)
		require.Equal(t, []string{"first", "z", "a", "x", "b"}, l.Values())
		require.True(t, l.Find("x"))
		require.False(t, l.Empty())
		l.PopIndex(2)
		l.PopIndex(100)
		l.PopBack()
		l.PopFront()
		require.Equal(t, []string{"z", "x"}, l.Values())
		var b bytes.Buffer
		l.Print(&b)
		require.Equal(t, "[z, x]\n", b.String())
		l.PopIndex(0)
		l.PopIndex(0)
		require.True(t, l.Empty())
		l.PushFront("again")
		l.PopBack()
		require.True(t, l.Empty())
		l.PushBack("again")
		l.PopFront()
		require.True(t, l.Empty())
	})
	t.Run("neighbors", func(t *testing.T) {
		l := newList()
		l.PushBack("a")
		l.PushBack("b")
		l.PushBack("c")
		require.True(t, l.InsertBefore("a", "first"))
		require.True(t, l.InsertBefore("b", "x"))
		require.True(t, l.InsertAfter("c", "last"))
		require.True(t, l.InsertAfter("a", "y"))
		require.True(t, l.EraseBefore("a"))
		require.True(t, l.EraseBefore("b"))
		require.True(t, l.EraseAfter("c"))
		require.True(t, l.EraseAfter("a"))
		require.Equal(t, []string{"a", "b", "c"}, l.Values())
		require.False(t, l.EraseBefore("a"))
		require.False(t, l.EraseAfter("c"))
		require.False(t, l.EraseAfter("missing"))
		require.False(t, l.EraseBefore("missing"))
		require.True(t, l.EraseValue("b"))
		require.True(t, l.EraseValue("a"))
		require.True(t, l.EraseValue("c"))
		require.True(t, l.Empty())
	})
}
func TestList(t *testing.T) { checkList(t, func() testList { return &List{} }) }
func TestListPastEnd(t *testing.T) {
	var l List
	l.PushIndex("a", 50)
	l.PushIndex("b", 1)
	require.Equal(t, []string{"a", "b"}, l.Values())
}
