package structures

import (
	"bytes"
	"fmt"
	"github.com/stretchr/testify/require"
	"strconv"
	"testing"
)

func TestHashTable(t *testing.T) {
	for _, double := range []bool{false, true} {
		t.Run(fmt.Sprint(double), func(t *testing.T) {
			h := NewHash(double)
			require.False(t, h.Has("a"))
			_, ok := h.Get("a")
			require.False(t, ok)
			h.Set("a", "red")
			h.Set("a", "green")
			v, ok := h.Get("a")
			require.True(t, ok)
			require.Equal(t, "green", v)
			require.Len(t, h.Names(), 1)
			h.Set("", "")
			h.Set("-", "minus")
			h.Set("+", "plus")
			v, ok = h.Get("-")
			require.True(t, ok)
			require.Equal(t, "minus", v)
			var out bytes.Buffer
			h.Print(&out)
			require.Contains(t, out.String(), `"a": "green"`)
			h.Del("a")
			h.Del("missing")
			require.False(t, h.Has("a"))
			for i := 0; i < 300; i++ {
				h.Set(strconv.Itoa(i), strconv.Itoa(i))
			}
			for i := 0; i < 300; i += 2 {
				h.Del(strconv.Itoa(i))
			}
			for i := 0; i < 300; i++ {
				v, found := h.Get(strconv.Itoa(i))
				require.Equal(t, i%2 != 0, found)
				if found {
					require.Equal(t, strconv.Itoa(i), v)
				}
			}
		})
	}
}
func TestHashDeletedCollision(t *testing.T) {
	for _, double := range []bool{false, true} {
		h := NewHash(double)
		var keys []string
		for i := 0; len(keys) < 4; i++ {
			k := strconv.Itoa(i)
			var hash uint64 = 14695981039346656037
			for j := 0; j < len(k); j++ {
				hash ^= uint64(k[j])
				hash *= 1099511628211
			}
			if hash%8 == 0 {
				keys = append(keys, k)
				h.Set(k, k)
			}
		}
		h.Del(keys[0])
		for _, k := range keys[1:] {
			v, ok := h.Get(k)
			require.True(t, ok)
			require.Equal(t, k, v)
		}
		h.Set(keys[0], "again")
		v, ok := h.Get(keys[0])
		require.True(t, ok)
		require.Equal(t, "again", v)
	}
}
func TestHashCleanup(t *testing.T) {
	h := NewHash(true)
	for i := 0; i < 100; i++ {
		k := strconv.Itoa(i)
		h.Set(k, k)
		h.Del(k)
	}
	require.Empty(t, h.Names())
	h.Set("last", "value")
	require.True(t, h.Has("last"))
}
