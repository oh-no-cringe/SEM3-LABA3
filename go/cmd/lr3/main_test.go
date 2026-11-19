package main

import (
	"bytes"
	"github.com/stretchr/testify/require"
	"strings"
	"testing"
)

func TestRun(t *testing.T) {
	var out, errors bytes.Buffer
	require.Equal(t, 0, run(strings.NewReader("1 PUSH a EXIT"), &out, &errors))
	require.Empty(t, errors.String())
	require.Contains(t, out.String(), "[a]")
	require.Equal(t, 1, run(strings.NewReader("9"), &out, &errors))
	require.Contains(t, errors.String(), "Ошибка:")
}
