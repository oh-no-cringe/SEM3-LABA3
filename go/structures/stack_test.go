package structures

import (
	"bytes"
	"github.com/stretchr/testify/require"
	"testing"
)

func TestStack(t *testing.T) {
	var s Stack
	require.True(t, s.Empty())
	_, err := s.Pop()
	require.Error(t, err)
	s.Push("a")
	s.Push("b")
	require.Equal(t, []string{"b", "a"}, s.Values())
	var output bytes.Buffer
	s.Print(&output)
	require.Equal(t, "[b, a]\n", output.String())
	value, err := s.Pop()
	require.NoError(t, err)
	require.Equal(t, "b", value)
	value, err = s.Pop()
	require.NoError(t, err)
	require.Equal(t, "a", value)
	require.True(t, s.Empty())
	s.Push("")
	value, err = s.Pop()
	require.NoError(t, err)
	require.Empty(t, value)
}
