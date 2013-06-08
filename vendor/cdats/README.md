# cdats

My implementation of some data structures.

* Linked List
* Hash Table
* Trie

## Testing

### All Tests

```sh
make ta
```

### Tasks for one data structure - list, hash, trie

There is a set of 3 tasks per data structure to help development: test, debug, leak

* **test** - this will run only the tests for this data structure 

* **debug** - this will call gdb loading the suite with the tests for this data structure

* **leak** - this will run valgrind with the tests for this data structure

Example:

```sh
make test name
```

