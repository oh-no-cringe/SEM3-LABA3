package structures

import (
	"bytes"
	"github.com/stretchr/testify/require"
	"strconv"
	"testing"
)

func TestArrayGrow(t *testing.T) {
	var a Array
	require.True(t, a.Empty())
	for i := 0; i < 100; i++ {
		a.PushBack(strconv.Itoa(i))
	}
	for i := 0; i < 100; i++ {
		v, err := a.Get(i)
		require.NoError(t, err)
		require.Equal(t, strconv.Itoa(i), v)
	}
	require.Equal(t, 100, a.Length())
}
func TestArrayChanges(t *testing.T) {
	var a Array
	for _, s := range []string{"a", "b", "c", "d"} {
		a.PushBack(s)
	}
	require.NoError(t, a.PushIndex("x", 1))
	require.NoError(t, a.Set(0, "z"))
	a.PopIndex(2)
	require.Equal(t, []string{"z", "x", "c", "d"}, a.Values())
	var output bytes.Buffer
	a.Print(&output)
	require.Equal(t, "[z, x, c, d]\n", output.String())
	var b Array
	b.PushBack("other")
	a.Swap(&b)
	require.Equal(t, []string{"other"}, a.Values())
	require.Equal(t, 4, b.Length())
	a.Swap(&a)
	require.Equal(t, 1, a.Length())
	b.Clear()
	require.True(t, b.Empty())
	b.PushBack("new")
	require.Equal(t, []string{"new"}, b.Values())
}
func TestArrayIndexes(t *testing.T) {
	var a Array
	for _, i := range []int{-1, 0, 5} {
		_, err := a.Get(i)
		require.Error(t, err)
		require.Error(t, a.Set(i, "x"))
	}
	require.Error(t, a.PushIndex("x", -1))
	require.Error(t, a.PushIndex("x", 1))
	a.PopIndex(-1)
	a.PopIndex(0)
	require.NoError(t, a.PushIndex("first", 0))
	require.NoError(t, a.PushIndex("last", 1))
	a.PopIndex(99)
	a.PopIndex(0)
	a.PopIndex(0)
	require.True(t, a.Empty())
}
