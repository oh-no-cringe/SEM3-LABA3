package structures

import (
	"github.com/stretchr/testify/require"
	"os"
	"path/filepath"
	"strconv"
	"testing"
)

func TestStorage(t *testing.T) {
	var a, b Array
	a.PushBack("")
	a.PushBack("строка\nс пробелами")
	a.PushBack("quote\"\\")
	var f, g ForwardList
	f.PushBack("a")
	f.PushBack("b")
	var l, m List
	l.PushBack("a")
	l.PushBack("b")
	var s, u Stack
	s.Push("first")
	s.Push("last")
	var q, r Queue[string]
	q.Push("first")
	q.Push("last")
	var x, y Tree
	for _, n := range []int{10, -20, 30, 40} {
		x.Insert(n)
	}
	if strconv.IntSize == 64 {
		large := int64(2147483648)
		x.Insert(int(large))
	}
	h, j := NewHash(true), NewHash(false)
	h.Set("a", "red")
	cases := []struct {
		first, second any
		kind          string
	}{{&a, &b, "array"}, {&f, &g, "list"}, {&l, &m, "list"}, {&s, &u, "stack"}, {&q, &r, "queue"}, {&x, &y, "tree"}, {h, j, "hash"}}
	for _, c := range cases {
		for _, bin := range []bool{false, true} {
			p := filepath.Join(t.TempDir(), "archive")
			require.NoError(t, SaveFile(c.first, p, c.kind, bin))
			require.NoError(t, LoadFile(c.second, p, c.kind, bin))
			v, e := snapshot(c.first)
			require.NoError(t, e)
			w, e := snapshot(c.second)
			require.NoError(t, e)
			require.Equal(t, v, w)
			require.Error(t, LoadFile(c.second, p, "wrong", bin))
			data, e := os.ReadFile(p)
			require.NoError(t, e)
			for _, bad := range [][]byte{[]byte("BAD"), data[:len(data)-1], append(append([]byte{}, data...), 1)} {
				require.NoError(t, os.WriteFile(p, bad, 0600))
				require.Error(t, LoadFile(c.second, p, c.kind, bin))
			}
		}
	}
	var empty Array
	p := filepath.Join(t.TempDir(), "empty")
	require.NoError(t, SaveFile(&empty, p, "array", false))
	require.NoError(t, LoadFile(&b, p, "array", false))
	require.True(t, b.Empty())
	require.Error(t, SaveFile(&empty, p+"/missing", "array", true))
	require.Error(t, LoadFile(&b, p+"missing", "array", true))
	require.Error(t, SaveFile(1, p, "array", false))
}
