package structures

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

func writeNumber(w io.Writer, n uint32, bin bool) error {
	if bin {
		return binary.Write(w, binary.LittleEndian, n)
	}
	_, err := fmt.Fprintln(w, n)
	return err
}
func readNumber(r *bufio.Reader, bin bool) (uint32, error) {
	var n uint32
	if bin {
		err := binary.Read(r, binary.LittleEndian, &n)
		return n, err
	}
	s, err := r.ReadString('\n')
	if err != nil {
		return 0, err
	}
	s = strings.TrimSuffix(s, "\n")
	if s == "" || strings.IndexFunc(s, func(c rune) bool { return c < '0' || c > '9' }) >= 0 {
		return 0, fmt.Errorf("неверное число")
	}
	value, err := strconv.ParseUint(s, 10, 32)
	return uint32(value), err
}
func writeString(w io.Writer, s string, bin bool) error {
	if len(s) > 1048576 {
		return fmt.Errorf("слишком длинная строка")
	}
	if err := writeNumber(w, uint32(len(s)), bin); err != nil {
		return err
	}
	if _, err := io.WriteString(w, s); err != nil {
		return err
	}
	if !bin {
		_, err := io.WriteString(w, "\n")
		return err
	}
	return nil
}
func readString(r *bufio.Reader, bin bool) (string, error) {
	n, err := readNumber(r, bin)
	if err != nil {
		return "", err
	}
	if n > 1048576 {
		return "", fmt.Errorf("слишком длинная строка")
	}
	data := make([]byte, n)
	if _, err = io.ReadFull(r, data); err != nil {
		return "", err
	}
	if !bin {
		c, err := r.ReadByte()
		if err != nil || c != '\n' {
			return "", fmt.Errorf("нет разделителя")
		}
	}
	return string(data), nil
}
func snapshot(object any) ([]string, error) {
	switch o := object.(type) {
	case *Array:
		return o.Values(), nil
	case *ForwardList:
		return o.Values(), nil
	case *List:
		return o.Values(), nil
	case *Stack:
		return o.Values(), nil
	case *Queue[string]:
		return o.Values(), nil
	case *Tree:
		return o.Values(), nil
	case *HashTable:
		mode := "0"
		if o.double {
			mode = "1"
		}
		v := []string{mode}
		for _, key := range o.Names() {
			value, _ := o.Get(key)
			v = append(v, key, value)
		}
		return v, nil
	default:
		return nil, fmt.Errorf("неизвестная структура")
	}
}
func restore(object any, v []string) error {
	switch o := object.(type) {
	case *Array:
		o.Clear()
		for _, s := range v {
			o.PushBack(s)
		}
	case *ForwardList:
		*o = ForwardList{}
		for _, s := range v {
			o.PushBack(s)
		}
	case *List:
		*o = List{}
		for _, s := range v {
			o.PushBack(s)
		}
	case *Stack:
		*o = Stack{}
		for i := len(v); i > 0; i-- {
			o.Push(v[i-1])
		}
	case *Queue[string]:
		*o = Queue[string]{}
		for _, s := range v {
			o.Push(s)
		}
	case *Tree:
		var temp Tree
		for _, s := range v {
			n, err := strconv.Atoi(s)
			if err != nil {
				return err
			}
			temp.Insert(n)
		}
		*o = temp
	case *HashTable:
		if len(v) == 0 || len(v)%2 == 0 || (v[0] != "0" && v[0] != "1") {
			return fmt.Errorf("неверная таблица")
		}
		temp := NewHash(v[0] == "1")
		for i := 1; i < len(v); i += 2 {
			if temp.Has(v[i]) {
				return fmt.Errorf("дубликат ключа")
			}
			temp.Set(v[i], v[i+1])
		}
		*o = *temp
	default:
		return fmt.Errorf("неизвестная структура")
	}
	return nil
}

// SaveFile сохраняет порядок элементов и режим хеширования в общий формат C++/Go.
func SaveFile(object any, path, kind string, bin bool) error {
	values, err := snapshot(object)
	if err != nil {
		return err
	}
	if len(values) > 100000 {
		return fmt.Errorf("слишком много элементов")
	}
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()
	header := "LR3TEXT1\n"
	if bin {
		header = "LR3BIN1\n"
	}
	if _, err = io.WriteString(f, header); err != nil {
		return err
	}
	if err = writeString(f, kind, bin); err != nil {
		return err
	}
	if err = writeNumber(f, uint32(len(values)), bin); err != nil {
		return err
	}
	for _, s := range values {
		if err = writeString(f, s, bin); err != nil {
			return err
		}
	}
	return f.Close()
}

// LoadFile сначала читает и проверяет файл, затем восстанавливает структуру.
func LoadFile(object any, path, kind string, bin bool) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()
	r := bufio.NewReader(f)
	header, err := r.ReadString('\n')
	want := "LR3TEXT1\n"
	if bin {
		want = "LR3BIN1\n"
	}
	if err != nil || header != want {
		return fmt.Errorf("неверный формат")
	}
	tag, err := readString(r, bin)
	if err != nil {
		return err
	}
	if tag != kind {
		return fmt.Errorf("другой тип структуры")
	}
	n, err := readNumber(r, bin)
	if err != nil {
		return err
	}
	if n > 100000 {
		return fmt.Errorf("слишком много элементов")
	}
	values := []string{}
	for i := uint32(0); i < n; i++ {
		s, err := readString(r, bin)
		if err != nil {
			return err
		}
		values = append(values, s)
	}
	if _, err = r.ReadByte(); err != io.EOF {
		return fmt.Errorf("лишние данные")
	}
	return restore(object, values)
}
