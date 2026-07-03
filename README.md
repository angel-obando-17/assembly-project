# MIPS Assembler in C (from scratch)

A from-scratch reimplementation, in C, of a MIPS assembly → machine code translator — built on top of a generic hash map and linked list that I wrote myself, rather than relying on any external data structure library.

> Status: **in progress** — paused while working on my undergraduate thesis, will be resumed.

## Background

This started as an assignment for my Computer Architecture course, where I originally implemented a MIPS assembler in Python for convenience. I wanted to revisit it in C — partly to get closer to the metal, and partly because I wanted to implement the underlying data structures myself instead of using a library `map`/`dict`. C doesn't have generics built in, so building a reusable hash map meant solving that problem from scratch.

## What's implemented so far

- **`linkedlist.c` / `linkedlist.h`** — a generic singly linked list, used as the bucket structure for the hash map (separate chaining).
- **`map.c` / `map.h`** — a generic hash map built on top of it:
  - Type-erased keys and values (`void*` + `size_t` size), so the map works with any data type without C++-style templates.
  - A function-pointer-based API (`insert`, `get`, `contains`, `erase`, `resize`, `clear`, `empty`, `size`) attached directly to each `Map` instance.
  - `load_factor` and `resize` already defined, in preparation for automatic rehashing as the map grows.

## What's next

- [ ] Finish and stress-test the hash map (`insert`/`erase`/`resize` correctness, collision handling)
- [ ] Instruction parser: tokenize MIPS assembly lines (opcode + operands)
- [ ] Symbol table for labels, built on top of the hash map above
- [ ] Two-pass assembly: resolve labels/addresses on the first pass, emit machine code on the second
- [ ] Encode R-type, I-type, and J-type instructions to 32-bit machine code
- [ ] Test against the instruction set covered in the original Python version

## Why it's public despite being unfinished

The value here is in the data structures layer — a generic, type-erased hash map with a method-style API is a non-trivial thing to build correctly in C, and I'd rather show real, working, in-progress code than hide it until the whole assembler is done.

## Files

```
map.c / map.h               # generic hash map (separate chaining)
linkedlist.c / linkedlist.h # generic linked list (map's bucket implementation)
main.c                      # entry point / driver (work in progress)
```

## Author

**Angel Luis Obando Fajardo** — Electronic Engineering student, Pontificia Universidad Javeriana Cali
[LinkedIn](https://www.linkedin.com/in/angel-obando-827600241/)
