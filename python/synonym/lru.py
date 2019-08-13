class LRUNode(object):
    def __init__(self, key=None, value=None):
        self.next = self.prev = None
        self.value = value
        self.key = key

class LRU(object):
    """Least Recently Used"""
    def __init__(self, capacity):
        self.cap = capacity
        self.head = LRUNode()
        self.tail = LRUNode()
        self.mappings = {}

        self.head.next = self.tail
        self.tail.prev = self.head

    def _add(self, node):
        nextNode = self.head.next

        self.head.next = node
        node.prev = self.head

        nextNode.prev = node
        node.next = nextNode

    def _del(self, node):
        node.prev.next = node.next
        node.next.prev = node.prev

    def search(self, key):
        if key in self.mappings:
            self._del(self.mappings[key])
            self._add(self.mappings[key])
            return self.mappings[key].value
        return None

    def add(self, key, value):
        if key in self.mappings:
            self.mappings[key].value = value
            self._del(self.mappings[key])
            self._add(self.mappings[key])

        else:
            node = LRUNode(key, value)
            self._add(node)
            self.mappings[key] = node
            if self.cap == 0:
                self.mappings.pop(self.tail.prev.key)
                self._del(self.tail.prev)
            else:
                self.cap -= 1