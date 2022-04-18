/**
 * @internal
 * @file xorlist.c
 * @author Kyle Laker (kyle@laker.email)
 *
 * @copyright Copyright (c) 2022
 *
 * An implementation of an XOR Linked List.
 * "HMB" - Me when starting this
 *
 * @endinternal
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "list.h"

#define UNSAFE_PTR_TO_INT(ptr) ((uintptr_t)(ptr))

typedef struct node
{
    struct node *link;
    list_val_t *value;
} node_t;

/**
 * Struct that can be used to find a surrounding node (or just be used for
 * the included nodes).
 */
typedef struct
{
    node_t *prev;
    node_t *curr;
} node_pair_t;

/*
 * Prototypes for the utility functions.
 */

static node_t *calc_new_ptr(void *, void *, void *);
static node_t *list_next(node_t *, node_t *);
static node_t *list_prev(node_t *, node_t *);
static int add_at_node(list_t *, list_val_t, node_t *, node_t *);
static node_pair_t traverse_to_idx(list_t *, size_t);

/******
 * Exported Functions
 ******/

/**
 * @brief Initialize a list
 *
 * Creates a heap-allocated list_t. This list can be used immediately. The
 * returned list should not be passed to free directly and should be passed
 * to list_destroy(list_t *, element_destructor).
 */
list_t *list_create()
{
    list_t *list = malloc(sizeof(list_t));
    node_t *head = malloc(sizeof(node_t));
    node_t *tail = malloc(sizeof(node_t));

    if (!list || !head || !tail)
    {
        return NULL;
    }

    head->link = calc_new_ptr(NULL, NULL, tail);
    tail->link = calc_new_ptr(head, NULL, NULL);

    list->head = head;
    list->tail = tail;

    list->size = 0;

    return list;
}

/**
 * @brief Deconstruct a list
 *
 * This will properly tear down the list and the nodes in the list; however,
 * the individual items that are stored are not directly passed to free as
 * they may be stack-allocated or require more complicated tear-down. These
 * can be passed to the destroy argument which shoud accept a single
 * (void *) as an argument (for example, free(void *)).
 *
 * @param list The list to tear down
 * @param destroy A function to properly free list elements (or NULL)
 */
void list_destroy(list_t *list, void (*destroy)(void *))
{
    /* Destroy all remaining items in the list. */
    while (list->size > 0)
    {
        list_val_t *item = list_pop(list);
        if (destroy)
        {
            destroy(item);
        }
    }

    /* Free memory for list struct members. */
    free(list->head);
    free(list->tail);
    list->head = NULL;
    list->tail = NULL;
    free(list);
}

/**
 * @brief Add an item to the list at an index.
 *
 * @param list The list ot add the item to
 * @param idx  The index to insert at
 * @param value The value to add
 * @return int A non-zero value on failure
 */
int list_insert(list_t *list, size_t idx, list_val_t value)
{
    node_pair_t nodes = traverse_to_idx(list, idx);
    if (!nodes.prev || !nodes.curr)
    {
        return EXIT_FAILURE;
    }

    /*
     * If we're working from the back of the list, we need to go one node
     * further since we substract 1 from the `list->size - idx` calculation
     * in the traverse_to_idx function.
     */
    if (idx > list->size / 2)
    {
        return add_at_node(list, value,
                           nodes.curr, list_next(nodes.curr, nodes.prev));
    }

    return add_at_node(list, value, nodes.prev, nodes.curr);
}

/**
 * @brief Add an item to the tail of the list
 *
 * @param list The list to add to
 * @param value The value to add to the list
 * @return int A non-zero value on failure
 */
int list_append(list_t *list, list_val_t value)
{
    return add_at_node(list, value, list_prev(list->tail, NULL), list->tail);
}

/**
 * @brief Add an item to the end of the queue
 *
 * This is an alias for list_append(list_t *, list_val_t)
 *
 * @param list The list to add to
 * @param value The value to add to the list
 * @return int A non-zero value on failure
 */
int list_enqueue(list_t *list, list_val_t value)
{
    return list_append(list, value);
}

/**
 * @brief Add an item to the head of the list
 *
 * @param list The list to add to
 * @param value The value to add to the list
 * @return int A non-zero value on failure
 */
int list_prepend(list_t *list, list_val_t value)
{
    return add_at_node(list, value, list->head, list_next(list->head, NULL));
}

/**
 * @brief Add an item to the top of the stack
 *
 * This is an alias for list_prepend(list_t *, list_val_t)
 *
 * @param list The list to add to
 * @param value The value to push to the stack
 * @return int A non-zero value on failure
 */
int list_push(list_t *list, list_val_t value)
{
    return list_prepend(list, value);
}

/**
 * @brief Determine if the list is empty
 *
 * @param list The list
 * @return true When the list is empty
 * @return false If the list contains elements
 */
bool list_is_empty(list_t list)
{
    return list.size == 0;
}

/**
 * @brief Remove an item from the list.
 *
 * This removes an item from the list at an index and returns the item.
 *
 * @param list The list to remove from
 * @param idx The index to remove at
 * @return list_val_t The item at that index (or NULL for an invalid index)
 */
list_val_t list_delete(list_t *list, size_t idx)
{
    node_pair_t nodes = traverse_to_idx(list, idx);
    if (!nodes.prev || !nodes.curr)
    {
        return NULL;
    }

    node_t *prev = nodes.prev;
    node_t *curr = nodes.curr;
    node_t *next = list_next(curr, prev);

    /* Calculate the new links to nodes. */
    next->link = calc_new_ptr(prev, curr, next->link);
    prev->link = calc_new_ptr(prev->link, curr, next);

    /* Get the value to return. */
    list_val_t val = curr->value;

    free(curr);
    curr = NULL;
    list->size -= 1;

    return val;
}

/**
 * @brief Pop the top item from the stack
 *
 * This is equivalent to list_delete(list_t *, size_t) with an index of 0.
 *
 * @param list The list to remove from
 * @return list_val_t The item at the top of the stack (or NULL if empty)
 */
list_val_t list_pop(list_t *list)
{
    return list_delete(list, 0);
}

/**
 * @brief Remove the first item from the queue
 *
 * This is equivalent to list_delete(list_t *, size_t) with an index of 0.
 *
 * @param list The queue to remove from
 * @return list_val_t The first item in the queue (or NULL if empty)
 */
list_val_t list_dequeue(list_t *list)
{
    return list_delete(list, 0);
}

/**
 * @brief Get (without removing) the item at an index
 *
 * @param list The list to retrieve the item from
 * @param idx  The index to retrieve at
 * @return list_val_t The item to peek at
 */
list_val_t list_get(list_t list, size_t idx)
{
    if (idx >= list.size)
    {
        return NULL;
    }

    node_pair_t nodes = traverse_to_idx(&list, idx);
    if (!nodes.prev || !nodes.curr)
    {
        return NULL;
    }

    return nodes.curr->value;
}

/**
 * @brief Peek at the first item in the queue (or top of stack)
 *
 * @param list The stack/queue to peek at
 * @return list_val_t The item at the front of queue (or top of the stack) or
 *         NULL if empty
 */
list_val_t list_peek(list_t list)
{
    return list_get(list, 0);
}

/**
 * @brief Change the value stored at an index
 *
 * This modifies the node in-place to set the value contained to the given
 * value.
 *
 * @param list The list to modify
 * @param idx The index to modify
 * @param value The new value to set
 * @return list_val_t The previous at the given index (or NULL if the index
 *         is invalid)
 */
list_val_t list_set(list_t *list, size_t idx, list_val_t value)
{
    node_pair_t nodes = traverse_to_idx(list, idx);
    if (!nodes.curr)
    {
        return NULL;
    }

    list_val_t prior_value = nodes.curr->value;
    nodes.curr->value = value;

    return prior_value;
}

/**
 * @brief The size of the list.
 *
 * @param list The list to check the size of
 * @return size_t The current number of items in the list
 */
size_t list_size(list_t list)
{
    return list.size;
}

/**
 * @brief Remove an item from the list by value
 *
 * This deletes the first matching item from the list by value and provides the
 * index of the item in the list. It is the responsibility of the caller to
 * ensure the memory for the value is deallocated corrected.
 *
 * @param list The list to remove the item from
 * @param value The value to remove from the list
 * @return ssize_t The index where the item was previously (or -1 if not found)
 */
ssize_t list_remove(list_t *list, list_val_t value)
{
    ssize_t idx = list_find(*list, value);

    /*
     * If the index is invalid, return -1 and don't remove anything.
     */
    if (idx > list->size || idx < 0)
    {
        return -1;
    }

    list_delete(list, idx);

    return idx;
}

/**
 * @brief Reverse the list
 *
 * A very efficient operation to reverse the order of the list. The head and
 * the tail are swapped so that traversal happens in reverse order.
 *
 * @param list The list to reverse
 */
void list_reverse(list_t *list)
{
    list->head = (node_t *)(UNSAFE_PTR_TO_INT(list->head) ^ UNSAFE_PTR_TO_INT(list->tail));
    list->tail = (node_t *)(UNSAFE_PTR_TO_INT(list->head) ^ UNSAFE_PTR_TO_INT(list->tail));
    list->head = (node_t *)(UNSAFE_PTR_TO_INT(list->head) ^ UNSAFE_PTR_TO_INT(list->tail));
}

/**
 * @brief Get the index of an item in the list
 *
 * This finds the first matching item (by value) in the list and provides the
 * index of that item.
 *
 * @param list The list to search
 * @param value The value to search for
 * @return ssize_t The index of `value` or -1 if not found
 */
ssize_t list_find(list_t list, list_val_t value)
{
    node_t *curr = list_next(list.head, NULL);
    node_t *prev = list.head;
    size_t idx = 0;

    /* Traverse until we find the first node with the value*/
    while (curr != list.tail)
    {
        if (curr->value == value)
        {
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

/**
 * @brief Check if a value exists in the list
 *
 * This does a regular equality comparison by element of the list to see if
 * the given value exists in the list
 *
 * @param list The list to search in
 * @param value The value to search for
 * @return true If the value is found in the list
 * @return false If the value is not found in the list
 */
bool list_contains(list_t list, list_val_t value)
{
    node_t *curr = list.head;
    node_t *prev = NULL;

    while (curr != list.tail)
    {
        node_t *next_node = list_next(curr, prev);
        prev = curr;
        curr = next_node;
        if (curr->value == value)
        {
            return true;
        }
    }

    /* The item does not exist in the list. */
    return false;
}

/*****
 * Utility Functions
 *****/

/**
 * Calculates the new link for a node. Useful for insertions and removals.
 */
static node_t *calc_new_ptr(void *a, void *b, void *c)
{
    return (node_t *)(UNSAFE_PTR_TO_INT(a) ^ UNSAFE_PTR_TO_INT(b) ^ UNSAFE_PTR_TO_INT(c));
}

/**
 * Returns the next item in the list after curr.
 */
static node_t *list_next(node_t *curr, node_t *prev)
{
    return calc_new_ptr(prev, curr->link, NULL);
}

/**
 * Returns the previous item in the list before curr.
 */
static node_t *list_prev(node_t *curr, node_t *next)
{
    return calc_new_ptr(NULL, curr->link, next);
}

/**
 * Add a node with a given value between two given nodes.
 */
static int add_at_node(list_t *list, list_val_t value,
                       node_t *before, node_t *after)
{
    /* Allocate and initialize the new node. */
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node)
    {
        return EXIT_FAILURE;
    }

    new_node->value = value;

    /* Set the pointers to surrounding nodes. */
    new_node->link = calc_new_ptr(before, NULL, after);
    after->link = calc_new_ptr(before, new_node, after->link);
    before->link = calc_new_ptr(before->link, new_node, after);

    list->size += 1;

    return EXIT_SUCCESS;
}

/**
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
    if (idx > list->size)
    {
        node_pair_t all_null = {.prev = NULL, .curr = NULL};
        return all_null;
    }

    /*
     * Search from the end of the list for indexes after halfway.
     */
    if (idx <= list->size / 2)
    {
        starting_end = list->head;
        num_iter = idx;
    }
    else
    {
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

    for (int i = 0; i < num_iter; i++)
    {
        node_t *tmp = list_next(curr, prev);
        prev = curr;
        curr = tmp;
    }

    node_pair_t result = {.prev = prev, .curr = curr};
    return result;
}
