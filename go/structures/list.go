package structures

type listNode struct {
	value      string
	next, prev *listNode
}
type List struct{ head, tail *listNode }

func (l *List) PushFront(v string) {
	n := &listNode{value: v, next: l.head}
	if l.head != nil {
		l.head.prev = n
	} else {
		l.tail = n
	}
	l.head = n
}
func (l *List) PushBack(v string) {
	n := &listNode{value: v, prev: l.tail}
	if l.tail != nil {
		l.tail.next = n
	} else {
		l.head = n
	}
	l.tail = n
}
func (l *List) PushIndex(v string, index int) {
	if index < 0 {
		return
	}
	if index == 0 {
		l.PushFront(v)
		return
	}
	c := l.head
	for i := 0; i < index-1 && c != nil; i++ {
		c = c.next
	}
	if c == nil {
		l.PushBack(v)
		return
	}
	l.insertAfterNode(c, v)
}
func (l *List) unlink(c *listNode) {
	if c.prev != nil {
		c.prev.next = c.next
	} else {
		l.head = c.next
	}
	if c.next != nil {
		c.next.prev = c.prev
	} else {
		l.tail = c.prev
	}
	c.next = nil
	c.prev = nil
}
func (l *List) PopFront() {
	if l.head != nil {
		l.unlink(l.head)
	}
}
func (l *List) PopBack() {
	if l.tail != nil {
		l.unlink(l.tail)
	}
}
func (l *List) PopIndex(index int) {
	if index < 0 {
		return
	}
	c := l.head
	for i := 0; i < index && c != nil; i++ {
		c = c.next
	}
	if c != nil {
		l.unlink(c)
	}
}
func (l *List) findNode(v string) *listNode {
	for c := l.head; c != nil; c = c.next {
		if c.value == v {
			return c
		}
	}
	return nil
}
func (l *List) Find(v string) bool {
	return l.findNode(v) != nil
}
func (l *List) InsertBefore(target, v string) bool {
	c := l.findNode(target)
	if c == nil {
		return false
	}
	n := &listNode{value: v, next: c, prev: c.prev}
	if c.prev != nil {
		c.prev.next = n
	} else {
		l.head = n
	}
	c.prev = n
	return true
}
func (l *List) insertAfterNode(c *listNode, v string) {
	n := &listNode{value: v, next: c.next, prev: c}
	if c.next != nil {
		c.next.prev = n
	} else {
		l.tail = n
	}
	c.next = n
}
func (l *List) InsertAfter(target, v string) bool {
	c := l.findNode(target)
	if c == nil {
		return false
	}
	l.insertAfterNode(c, v)
	return true
}
func (l *List) EraseValue(v string) bool {
	c := l.findNode(v)
	if c == nil {
		return false
	}
	l.unlink(c)
	return true
}
func (l *List) EraseBefore(target string) bool {
	c := l.findNode(target)
	if c == nil || c.prev == nil {
		return false
	}
	l.unlink(c.prev)
	return true
}
func (l *List) EraseAfter(target string) bool {
	c := l.findNode(target)
	if c == nil || c.next == nil {
		return false
	}
	l.unlink(c.next)
	return true
}
func (l *List) Empty() bool {
	return l.head == nil
}
func (l *List) Values() []string {
	v := []string{}
	for c := l.head; c != nil; c = c.next {
		v = append(v, c.value)
	}
	return v
}
