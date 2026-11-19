package structures

type HashTable struct {
	keys, data  Array
	count, used int
	double      bool
}

func NewHash(double bool) *HashTable {
	table := &HashTable{double: double}
	for i := 0; i < 8; i++ {
		table.keys.PushBack("")
		table.data.PushBack("")
	}
	return table
}

func (t *HashTable) hashStep(key string) int {
	if !t.double {
		return 1
	}
	var number uint64
	for i := 0; i < len(key); i++ {
		number = number*131 + uint64(key[i])
	}
	// Нечётный шаг проходит все ячейки при размере 8, 16, 32 и далее.
	return 2*int(number%uint64(t.keys.Length()/2)) + 1
}

func (t *HashTable) findPos(key string) int {
	size := t.keys.Length()
	// FNV-1a вычисляет начальную позицию в обоих режимах.
	var hash uint64 = 14695981039346656037
	for i := 0; i < len(key); i++ {
		hash ^= uint64(key[i])
		hash *= 1099511628211
	}
	pos := int(hash % uint64(size))
	jump := t.hashStep(key)
	deleted := size
	wanted := "+" + key

	for i := 0; i < size; i++ {
		cell := t.keys.data[pos]
		if cell == wanted {
			return pos
		}
		if cell == "" {
			if deleted != size {
				return deleted
			}
			return pos
		}
		if cell == "-" && deleted == size {
			deleted = pos
		}
		pos = (pos + jump) % size
	}
	return deleted
}

func (t *HashTable) Set(key, value string) {
	pos := t.findPos(key)
	size := t.keys.Length()
	if pos < size && t.keys.data[pos] == "+"+key {
		t.data.data[pos] = value
		return
	}
	if t.count >= size/2 || pos == size {
		t.rebuild(true)
		pos = t.findPos(key)
	} else if t.used >= size-size/4 && t.keys.data[pos] == "" {
		t.rebuild(false)
		pos = t.findPos(key)
	}

	if t.keys.data[pos] == "" {
		t.used++
	}
	t.keys.data[pos] = "+" + key
	t.data.data[pos] = value
	t.count++
}

func (t *HashTable) Get(key string) (string, bool) {
	pos := t.findPos(key)
	if pos == t.keys.Length() || t.keys.data[pos] != "+"+key {
		return "", false
	}
	return t.data.data[pos], true
}

func (t *HashTable) Has(key string) bool {
	_, found := t.Get(key)
	return found
}

func (t *HashTable) Del(key string) {
	pos := t.findPos(key)
	if pos < t.keys.Length() && t.keys.data[pos] == "+"+key {
		t.keys.data[pos] = "-"
		t.data.data[pos] = ""
		t.count--
	}
}

func (t *HashTable) Names() []string {
	names := []string{}
	for i := 0; i < t.keys.Length(); i++ {
		key := t.keys.data[i]
		if len(key) > 0 && key[0] == '+' {
			names = append(names, key[1:])
		}
	}
	return names
}

func (t *HashTable) rebuild(grow bool) {
	var keys, values Array
	size := t.keys.Length()
	for i := 0; i < size; i++ {
		cell := t.keys.data[i]
		if len(cell) > 0 && cell[0] == '+' {
			keys.PushBack(cell[1:])
			values.PushBack(t.data.data[i])
		}
		t.keys.data[i] = ""
		t.data.data[i] = ""
	}
	if grow {
		for i := 0; i < size; i++ {
			t.keys.PushBack("")
			t.data.PushBack("")
		}
	}
	t.count = 0
	t.used = 0
	for i := 0; i < keys.Length(); i++ {
		t.Set(keys.data[i], values.data[i])
	}
}
