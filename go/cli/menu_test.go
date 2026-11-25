package cli

import (
	"bytes"
	"github.com/stretchr/testify/require"
	"path/filepath"
	"strings"
	"testing"
)

func TestCommands(t *testing.T) {
	cases := []struct{ input, want string }{
		{"1 EMPTY PUSH a PUSH b INSERT 1 c GET 1 SET 0 z SIZE DEL 1 PRINT EXIT", "[z, b]"},
		{"2 EMPTY PUSH a PUSH b FRONT z INSERT 1 c FIND c FIND missing DEL 1 DEL_VALUE a POP_BACK POP_FRONT PRINT EXIT", "[]"},
		{"3 EMPTY PUSH a PUSH b FRONT z INSERT 1 c FIND c DEL 1 DEL_VALUE a POP_BACK POP_FRONT PRINT EXIT", "[]"},
		{"4 POP EMPTY PUSH a PUSH b POP PRINT POP EMPTY EXIT", "TRUE"},
		{"5 POP EMPTY PUSH a PUSH b POP PRINT POP EMPTY EXIT", "TRUE"},
		{"6 1 SET a red SET b blue GET a GET b GET missing HAS a HAS missing DEL a PRINT EXIT", "blue"},
		{"6 2 SET a red GET a DEL a HAS a EXIT", "FALSE"},
		{"7 EMPTY CHECK PRINT INSERT 10 INSERT 20 INSERT 30 INSERT 40 FIND 40 FIND 99 PRE IN POST CHECK EXIT", "[40, 20, 30, 10]"},
		{`1 PUSH "hello world" GET 0 EXIT`, "hello world"},
		{`1 PUSH "a\"b" PRINT EXIT`, `a"b`},
	}
	for _, tc := range cases {
		var out bytes.Buffer
		require.NoError(t, Run(strings.NewReader(tc.input), &out), tc.input)
		require.Contains(t, out.String(), tc.want)
	}
	for _, prefix := range []string{"1", "2", "3", "4", "5", "6 1", "7"} {
		var out bytes.Buffer
		require.NoError(t, Run(strings.NewReader(prefix), &out))
	}
}
func TestErrors(t *testing.T) {
	for _, input := range []string{
		"", "x", "9", "1 WHAT", "1 GET 9", "1 GET -1", "1 GET x", "1 GET", "1 SET 0 a", "1 INSERT 9 a", "1 INSERT", "1 INSERT 0", "1 PUSH", "1 DEL", "1 DEL -1",
		"2 WHAT", "2 PUSH", "2 INSERT", "2 INSERT 0", "2 DEL", "3 FIND", "4 WHAT", "4 PUSH", "5 WHAT", "6", "6 x", "6 9", "6 1 WHAT", "6 1 GET", "6 1 SET a", "7 WHAT", "7 INSERT x", "7 FIND",
		`1 PUSH "unclosed`, `1 PUSH "a"b`, `1 PUSH "a\`,
	} {
		var out bytes.Buffer
		require.Error(t, Run(strings.NewReader(input), &out), input)
	}
}

func TestFiles(t *testing.T) {
	for _, c := range []struct{ prefix, populate string }{
		{"1", "PUSH a PUSH b"}, {"2", "PUSH a PUSH b"},
		{"3", "PUSH a PUSH b"}, {"4", "PUSH a PUSH b"},
		{"5", "PUSH a PUSH b"}, {"6 2", "SET apple red"},
		{"7", "INSERT 10 INSERT 20"},
	} {
		for _, format := range []string{"text", "binary"} {
			path := filepath.Join(t.TempDir(), "archive")
			input := c.prefix + " " + c.populate + " SAVE " + format + " " + path + " LOAD " + format + " " + path + " PRINT EXIT"
			var out bytes.Buffer
			require.NoError(t, Run(strings.NewReader(input), &out))
			require.Contains(t, out.String(), "OK")
		}
		for _, command := range []string{"SAVE json unused", "LOAD text missing", "SAVE", "SAVE text"} {
			var out bytes.Buffer
			require.Error(t, Run(strings.NewReader(c.prefix+" "+command), &out))
		}
	}
}
