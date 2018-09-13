/*
 * An implementation of an XOR Linked List.
 * "HMB" - Me when starting this
 *
 *
 * Copyright 2018 Kyle Laker
 *
 * This file is licensed under the terms of the MIT License
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "list.h"

/*
 * Struct that can be used to find a surrounding node (or just be used for
 * the included nodes).
 */
typedef struct {
    node_t *prev;
    node_t *curr;
} node_pair_t;


/*
 * Prototypes for the utility functions.
 */

static node_t      *calc_new_ptr(void *, void *, void *);
static node_t      *list_next(node_t *, node_t *);
static node_t      *list_prev(node_t *, node_t *);
static bool         add_at_node(list_t *, list_val_t, node_t *, node_t *);
static node_pair_t  traverse_to_idx(list_t *, size_t);


/******
 * Exported Functions
 ******/

/*
 * Creates an instance of the list allocated on the heap. The list must be
 * destroyed by calling the list_destroy() function.
 */
list_t *list_create()
{
    list_t *list = malloc(sizeof(list_t));
    node_t *head = malloc(sizeof(node_t));
    node_t *tail = malloc(sizeof(node_t));

    if (!list || !head || !tail) {
        return NULL;
    }

    head->link = calc_new_ptr(NULL, NULL, tail);
    tail->link = calc_new_ptr(head, NULL, NULL);

    list->head = head;
    list->tail = tail;

    list->size = 0;

    return list;
}

/*
 * Removes all items from the list and then free()'s the memory for the list
 * itself. The list may not be used after callng this function.
 */
void list_destroy(list_t *list)
{
    /* Destroy all remaining items in the list. */
    while (list->size > 0) {
        list_delete(list, 0);
    }

    /* Free memory for list struct members. */
    free(list->head);
    free(list->tail);
    list->head = NULL;
    list->tail = NULL;
    free(list);
}

/*
 * Adds value to list at idx. Returns true if the item was successfully added
 * to the list; returns false otherwise.
 */
bool list_insert(list_t *list, size_t idx, list_val_t value)
{
    node_pair_t nodes = traverse_to_idx(list, idx);
    if (!nodes.prev || !nodes.curr) {
        return false;
    }

    /*
     * If we're working from the back of the list, we need to go one node
     * further since we substract 1 from the `list->size - idx` calculation
     * in the traverse_to_idx function.
     */
    if (idx > list->size / 2) {
        return add_at_node(list, value,
                           nodes.curr, list_next(nodes.curr, nodes.prev));
    }

    return add_at_node(list, value, nodes.prev, nodes.curr);
}

/*
 * Adds an item to the end of list. Returns true if the item was added
 * successfully; returns false otherwise.
 */
bool list_append(list_t *list, list_val_t value)
{
    return add_at_node(list, value, list_prev(list->tail, NULL), list->tail);
}

/*
 * Enqueues the item. Alias for list_append
 */
bool list_enqueue(list_t *list, list_val_t value)
{
    return list_append(list, value);
}

/*
 * Adds an item to the beginning of list. Returns true if the item was added
 * successfully; returns false otherwise.
 */
bool list_prepend(list_t *list, list_val_t value)
{
    return add_at_node(list, value, list->head, list_next(list->head, NULL));
}

/*
 * Pushes the item to the list. Alias for list_prepend
 */
bool list_push(list_t *list, list_val_t value)
{
    return list_prepend(list, value);
}

/* Returns true if the list is empty, false otherwise. */
bool list_is_empty(list_t list)
{
    return list.size == 0;
}

/* Removes and returns the item located at idx in list. */
list_val_t list_delete(list_t *list, size_t idx)
{
    node_pair_t nodes = traverse_to_idx(list, idx);
    if (!nodes.prev || !nodes.curr) {
        return false;
    }

    node_t *prev = nodes.prev;
    node_t *curr = nodes.curr;
    node_t *next = list_next(curr, prev);

    /* Calculate the new links to nodes. */
    next->link = calc_new_ptr(prev,       curr, next->link);
    prev->link = calc_new_ptr(prev->link, curr, next);

    /* Get the value to return. */
    list_val_t val = curr->value;

    free(curr);
    curr = NULL;
    list->size -= 1;

    return val;
}

/*
 * Removes and returns the first item from the front of the list
 */
list_val_t list_pop(list_t *list)
{
    return list_delete(list, 0);
}

/*
 * Removes and returns the last item from the list.
 */
list_val_t list_dequeue(list_t *list)
{
    return list_delete(list, 0);
}

/* Returns the item at idx in list. */
list_val_t list_get(list_t list, size_t idx)
{
    if (idx >= list.size) {
        return NULL;
    }

    node_pair_t nodes = traverse_to_idx(&list, idx);
    if (!nodes.prev || !nodes.curr) {
        return NULL;
    }

    return nodes.curr->value;
}

/*
 * Returns the item from the front of the list
 */
list_val_t list_peek(list_t list)
{
    return list_get(list, 0);
}

/* Changes the value at a selected index. */
bool list_set(list_t *list, size_t idx, list_val_t value)
{
    node_pair_t nodes = traverse_to_idx(list, idx);
    if (!nodes.curr) {
        return false;
    }

    nodes.curr->value = value;

    return true;
}

/* Provides the size of the list. */
size_t list_size(list_t list)
{
    return list.size;
}

/* Remove an item from the list. */
ssize_t list_remove(list_t *list, list_val_t value)
{
    ssize_t idx = list_find(*list, value);

    /*
     * If the index is invalid, return -1 and don't remove anything.
     */
    if (idx > list->size || idx < 0) {
        return -1;
    }

    list_delete(list, idx);

    return idx;
}

/*
 * Provides the index of value in the list. Returns -1 if value is not in the
 * list.
 */
ssize_t list_find(list_t list, list_val_t value)
{
    node_t *curr = list_next(list.head, NULL);
    node_t *prev = list.head;
    size_t idx = 0;

    /* Traverse until we find the first node with the value*/
    while (curr != list.tail) {
        if (curr->value == value) {
            return idx;
        }
        node_t *next_node = list_next(curr, prev);
        prev = curr;
        curr = next_node;
        idx++;
    }

    /* The item does not exist in the list. */
    return -1;
}

/*
 * Checks whether the provided list contains the value provided. Returns true
 * if value is in the list, returns false if it is not.
 */
bool list_contains(list_t list, list_val_t value)
{
    node_t *curr = list.head;
    node_t *prev = NULL;

    while (curr != list.tail) {
        node_t *next_node = list_next(curr, prev);
        prev = curr;
        curr = next_node;
        if (curr->value == value) {
            return true;
        }
    }

    /* The item does not exist in the list. */
    return false;
}


/*****
 * Utility Functions
 *****/

/*
 * Calculates the new link for a node. Useful for insertions and removals.
 */
static node_t *calc_new_ptr(void *a, void *b, void *c)
{
    return (node_t *)((intptr_t)(a) ^ (intptr_t)(b) ^ (intptr_t)(c));
}

/*
 * Returns the next item in the list after curr.
 */
static node_t *list_next(node_t *curr, node_t *prev)
{
    return calc_new_ptr(prev, curr->link, NULL);
}

/*
 * Returns the previous item in the list before curr.
 */
static node_t *list_prev(node_t *curr, node_t *next)
{
    return calc_new_ptr(NULL, curr->link, next);
}

/*
 * Add a node with a given value between two given nodes.
 */
static bool add_at_node(list_t *list, list_val_t value,
                        node_t *before, node_t *after)
{
    /* Allocate and initialize the new node. */
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) {
        return false;
    }

    new_node->value = value;

    /* Set the pointers to surrounding nodes. */
    new_node->link = calc_new_ptr(before,       NULL,     after);
    after->link    = calc_new_ptr(before,       new_node, after->link);
    before->link   = calc_new_ptr(before->link, new_node, after);

    list->size += 1;

    return true;
}

/*
 * Traverses the list and returns the node at the specified index. This is
 * only useful when the index is actually known.
 */
static node_pair_t traverse_to_idx(list_t *list, size_t idx)
{
    node_t *starting_end;
    size_t num_iter;

    /*
     * If the index isn't valid, don't bother searching at all.
     */
    if (idx > list->size) {
        node_pair_t all_null = { .prev = NULL, .curr = NULL };
        return all_null;
    }

    /*
     * Search from the end of the list for indexes after halfway.
     */
    if (idx <= list->size / 2) {
        starting_end = list->head;
        num_iter = idx;
    } else {
        starting_end = list->tail;
        /*
         * Subtrace one so to fix index calculations for accesses. This does
         * present a small issue when adding new nodes, but it's a simple
         * workaround.
         */
        num_iter = list->size - idx - 1;
    }

    /* Start traversing from the end until the needed index. */
    node_t *curr = list_next(starting_end, NULL);
    node_t *prev = starting_end;

    for (int i = 0; i < num_iter; i++) {
        node_t *tmp = list_next(curr, prev);
        prev = curr;
        curr = tmp;
    }

    node_pair_t result = { .prev = prev, .curr = curr };
    return result;
}
