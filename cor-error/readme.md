## Error Correction
Correct Words either chinese or english

### requirement
* pybind11

### How to use
```
cd lib
make
```

* use python
```python
import bktree_pybind as bktree

t = bktree.tree()
t.add("hello", 2.2)
t.add("hi", 2.2)

t.search("hell", 2)
```