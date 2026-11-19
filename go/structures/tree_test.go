package structures

import (
	"bytes"
	"github.com/stretchr/testify/require"
	"testing"
)

func TestTree(t *testing.T) {
	var tree Tree
	require.True(t, tree.Empty())
	require.True(t, tree.IsComplete())
	require.False(t, tree.Search(10))
	require.Empty(t, tree.Values())
	for _, v := range []int{10, 20, 30, 40} {
		tree.Insert(v)
		require.True(t, tree.IsComplete())
	}
	require.False(t, tree.Empty())
	require.True(t, tree.Search(40))
	require.True(t, tree.Search(30))
	require.False(t, tree.Search(99))
	require.Equal(t, []string{"10", "20", "40", "30"}, tree.Traverse("PRE"))
	require.Equal(t, []string{"40", "20", "10", "30"}, tree.Traverse("IN"))
	require.Equal(t, []string{"40", "20", "30", "10"}, tree.Traverse("POST"))
	var out bytes.Buffer
	tree.Print(&out)
	tree.PrintOrder(&out, "PRE")
	require.Equal(t, "[10, 20, 30, 40]\n[10, 20, 40, 30]\n", out.String())
}
func TestTreeGrow(t *testing.T) {
	var tree Tree
	for i := 0; i < 100; i++ {
		tree.Insert(i)
		require.True(t, tree.IsComplete())
	}
	require.Len(t, tree.Values(), 100)
}
func TestTreeGap(t *testing.T) {
	tree := Tree{root: &treeNode{value: 1, right: &treeNode{value: 2}}}
	require.False(t, tree.IsComplete())
}
