package structures

import (
	"github.com/stretchr/testify/require"
	"testing"
)

func TestForwardList(t *testing.T) { checkList(t, func() testList { return &ForwardList{} }) }
func TestForwardListPastEnd(t *testing.T) {
	var l ForwardList
	l.PushIndex("x", 5)
	require.True(t, l.Empty())
	l.PushBack("a")
	l.PushIndex("x", 5)
	require.Equal(t, []string{"a"}, l.Values())
	l.PopBack()
	require.True(t, l.Empty())
}
