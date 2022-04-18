# xorlist

An implementation of an XOR Linked List.

An XOR Linked List is a doubly-linked list that stores the pointers to the
next and the previous elements in the same field but XORed. Since XOR
never causes an overflow for integers, it is safer than using addition or
subtraction. All three of those operations are undefined or awkward for
pointers, so the uintptr_t is relied on heavily.

Such a linked list provides little value other than than requiring
sizeof(void *) less memory for each node and concealing the memory
addresses of the next and previous items in the list (if that's actually
a feature). In order to retrieve the address of a particular node N, the
addresses of either the two elements before N or the two elements after N
must be known.

This is *not* an intrusive list.

For example:

```
+------+    +---+    +---+    +---+    +------+
| HEAD |<-->| A |<-->| B |<-->| C |<-->| TAIL |
+------+    +---+    +---+    +---+    +------+
```

Finding B requires knowing the addresses of A and HEAD. It would be
accessed by performing (A->link ^ &HEAD). It could also be accessed by
(C->link ^ &TAIL) if traversing in reverse was desired. For the head and
tail of the list, the address of the next and previous elements respectively
are XORed with NULL.

In the above example:

| Field      | Value       |
|------------|-------------|
| HEAD->link | `NULL ^ &A` |
| A->link    | `&HEAD ^ &B`|
| B->link    | `&A ^ &B`   |
| C->link    | `&B ^ &TAIL`|
| TAIL->link | `&C ^ NULL` |

If an element D were inserted between B and C, the list would then look
like:

```
+------+    +---+    +---+    +---+    +---+    +------+
| HEAD |<-->| A |<-->| B |<-->| D |<-->| C |<-->| TAIL |
+------+    +---+    +---+    +---+    +---+    +------+
```

And the following links would be different:

| Field      | Value       |
|------------|-------------|
| B->link    | `&A ^ &D`   |
| D->link    | `&B ^ &C`   |
| C->link    | `&D ^ &TAIL`|

An update to add a new node, M, between two nodes P and Q essentially becomes:

```
P->link = P->link ^ &Q ^ &M
Q->link = Q->link ^ &P ^ &M
M->link = &P ^ &Q
```

Finally, if for some reason you wanted to reverse the list, doing so is trival.
In keeping with the spirit of trying to limit memory use, a simple XOR-swap is
all it takes to reverse the list (since traversal is purely based on the link
pointers):

```
HEAD->link ^= TAIL->link
TAIL->link ^= HEAD->link
HEAD->link ^= TAIL->link
```

## Installing

A typical `make && sudo make install` will work on most Linux distros (though
be careful! `/usr/` will be modified). For ArchLinux users, a `PKGBUILD` is
provided. You can build with `makepkg -si` to install the latest released
version. This package is not currently available in the Arch repos or the AUR.

## Notes

Pointers are not integers. This very heavily treats pointers as if they were
in fact integers. That's a horrible idea.