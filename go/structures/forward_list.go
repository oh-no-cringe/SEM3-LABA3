package structures

type forwardNode struct {
	value string
	next  *forwardNode
}
type ForwardList struct{ head *forwardNode }

func (l *ForwardList) PushFront(v string) {
	l.head = &forwardNode{v, l.head}
}
func (l *ForwardList) PushBack(v string) {
	n := &forwardNode{value: v}
	if l.head == nil {
		l.head = n
		return
	}
	c := l.head
	for c.next != nil {
		c = c.next
	}
	c.next = n
}
func (l *ForwardList) PushIndex(v string, index int) {
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
	if c != nil {
		c.next = &forwardNode{v, c.next}
	}
}
func (l *ForwardList) PopFront() {
	if l.head != nil {
		l.head = l.head.next
	}
}
func (l *ForwardList) PopBack() {
	if l.head == nil {
		return
	}
	if l.head.next == nil {
		l.head = nil
		return
	}
	c := l.head
	for c.next.next != nil {
		c = c.next
	}
	c.next = nil
}
func (l *ForwardList) PopIndex(index int) {
	if index < 0 {
		return
	}
	if index == 0 {
		l.PopFront()
		return
	}
	c := l.head
	for i := 0; i < index-1 && c != nil; i++ {
		c = c.next
	}
	if c != nil && c.next != nil {
		c.next = c.next.next
	}
}
func (l *ForwardList) Find(v string) bool {
	for c := l.head; c != nil; c = c.next {
		if c.value == v {
			return true
		}
	}
	return false
}
func (l *ForwardList) InsertBefore(target, v string) bool {
	var prev *forwardNode
	c := l.head
	for c != nil && c.value != target {
		prev = c
		c = c.next
	}
	if c == nil {
		return false
	}
	n := &forwardNode{v, c}
	if prev == nil {
		l.head = n
	} else {
		prev.next = n
	}
	return true
}
func (l *ForwardList) InsertAfter(target, v string) bool {
	c := l.head
	for c != nil && c.value != target {
		c = c.next
	}
	if c == nil {
		return false
	}
	c.next = &forwardNode{v, c.next}
	return true
}
func (l *ForwardList) EraseValue(v string) bool {
	var prev *forwardNode
	c := l.head
	for c != nil && c.value != v {
		prev = c
		c = c.next
	}
	if c == nil {
		return false
	}
	if prev == nil {
		l.head = c.next
	} else {
		prev.next = c.next
	}
	return true
}
func (l *ForwardList) EraseBefore(target string) bool {
	var before, prev *forwardNode
	c := l.head
	for c != nil && c.value != target {
		before = prev
		prev = c
		c = c.next
	}
	if c == nil || prev == nil {
		return false
	}
	if before == nil {
		l.head = c
	} else {
		before.next = c
	}
	return true
}
func (l *ForwardList) EraseAfter(target string) bool {
	c := l.head
	for c != nil && c.value != target {
		c = c.next
	}
	if c == nil || c.next == nil {
		return false
	}
	c.next = c.next.next
	return true
}
func (l *ForwardList) Empty() bool {
	return l.head == nil
}
func (l *ForwardList) Values() []string {
	v := []string{}
	for c := l.head; c != nil; c = c.next {
		v = append(v, c.value)
	}
	return v
}
