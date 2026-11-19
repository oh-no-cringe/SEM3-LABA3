package structures

import (
	"bytes"
	"github.com/stretchr/testify/require"
	"testing"
)

func TestQueue(t *testing.T) {
	var q Queue[string]
	require.True(t, q.Empty())
	_, err := q.Pop()
	require.Error(t, err)
	q.Push("a")
	q.Push("b")
	require.Equal(t, []string{"a", "b"}, q.Values())
	var output bytes.Buffer
	q.Print(&output)
	require.Equal(t, "[a, b]\n", output.String())
	v, err := q.Pop()
	require.NoError(t, err)
	require.Equal(t, "a", v)
	v, err = q.Pop()
	require.NoError(t, err)
	require.Equal(t, "b", v)
	require.True(t, q.Empty())
	q.Push("again")
	v, err = q.Pop()
	require.NoError(t, err)
	require.Equal(t, "again", v)
}
func TestQueuePointers(t *testing.T) {
	var q Queue[*int]
	number := 42
	q.Push(&number)
	q.Push(nil)
	v, err := q.Pop()
	require.NoError(t, err)
	require.Same(t, &number, v)
	v, err = q.Pop()
	require.NoError(t, err)
	require.Nil(t, v)
}
