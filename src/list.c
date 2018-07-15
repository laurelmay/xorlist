/*
 * An implementation of an XOR Linked List.
 * "HMB" - Me when starting this
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
 * Calculates the new link for a node. Useful for insertions and removals.
 */
static node_t *calc_new_ptr(void *a, void *b, void *c) {
    return (node_t *)((intptr_t)(a) ^ (intptr_t)(b) ^ (intptr_t)(c));
}

/*
 * Returns the next item in the list after curr.
 */
static node_t *list_next(node_t *curr, node_t *prev) {
    return calc_new_ptr(prev, curr->link, NULL);
}

/*
 * Returns the previous item in the list before curr.
 */
static node_t *list_prev(node_t *curr, node_t *next) {
    return calc_new_ptr(NULL, curr->link, next);
}

/*
 * Creates an instance of the list allocated on the heap. The list must be
 * free()'d by the caller. Returns a pointer to the created list.
 */
list_t *list_create() {
    list_t *list = calloc(1, sizeof(list_t));
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
 * itself.
 */
void list_destroy(list_t *list) {
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
bool list_insert(list_t *list, size_t idx, list_val_t value) {
    size_t count = idx;
    node_t *start_at = list->head;

    if (idx > list->size) {
        return false;
    }

    /* Start at the end if inserting closer to the end. */
    if (idx > list->size / 2) {
        count = list->size - idx;
        start_at = list->tail;
    }

    /* Initialize the new node */
    node_t *new_node = calloc(1, sizeof(node_t));
    if (!new_node) return false;

    new_node->value = value;

    node_t *next = list_next(start_at, NULL);
    node_t *prev = start_at;

    /* Find where the new node will be inserted */
    for (size_t i = 0; i < count; i++) {
        node_t *next_node = list_next(next, prev);
        prev = next;
        next = next_node;
    }

    /* Calculate the new links to previous and next nodes */
    new_node->link = calc_new_ptr(prev, NULL, next);
    prev->link = calc_new_ptr(prev->link, new_node, next);
    next->link = calc_new_ptr(prev, new_node, next->link);

    list->size += 1;

    return true;
}

/*
 * Adds an item to the end of list. Returns true if the item was added
 * successfully; returns false otherwise.
 */
bool list_append(list_t *list, list_val_t value) {
    node_t *new_node = calloc(1, sizeof(node_t));
    if (!new_node) return false;

    new_node->value = value;

    node_t *last = list_prev(list->tail, NULL);
    node_t *tail = list->tail;

    new_node->link = calc_new_ptr(last, NULL, tail);
    last->link = calc_new_ptr(last->link, new_node, tail);
    tail->link = calc_new_ptr(last, new_node, tail->link);

    list->size += 1;

    return true;
}

/* 
 * Adds an item to the beginning of list. Returns true if the item was added
 * successfully; returns false otherwise.
 */
bool list_prepend(list_t *list, list_val_t value) {
    return list_insert(list, 0, value);
}

/* Returns true if the list is empty, false otherwise. */
bool list_is_empty(list_t list) {
    return list.size == 0;
}

/* Removes and returns the item located at idx in list. */
list_val_t list_delete(list_t *list, size_t idx) {
    size_t count = idx;
    node_t *start_at = list->head;
    node_t *end_at = list->tail;

    if (idx > list->size) {
        return false;
    }

    /* Start at the end if removing closer to the end. */
    if (idx > list->size / 2) {
        count = list->size - idx - 1;
        start_at = list->tail;
        end_at = list->head;
    }

    /* Iterate to the index to remove */
    node_t *curr = list_next(start_at, NULL);
    node_t *prev = start_at;

    for (size_t i = 0; i < count; i++) {
        node_t *next_link = list_next(curr, prev);
        prev = curr;
        curr = next_link;
    }

    /* Ensure we didn't accidentally get to the tail. */
    node_t *next_link;
    if (curr == end_at) {
        next_link = NULL;
    } else {
        next_link = list_next(curr, prev);
    }

    /* Calculate the new links to nodes. */
    next_link->link = calc_new_ptr(prev, curr, next_link->link);
    prev-> link     = calc_new_ptr(prev->link, curr, next_link);

    /* Get the value to return. */
    list_val_t val = curr->value;

    free(curr);
    list->size -= 1;

    return val;
}

/* Returns the item at idx in list. */
list_val_t list_get(list_t list, size_t idx) {
    node_t *start_at = list.head;
    size_t count = idx;

    if (idx >= list.size) {
        return -1;
    }

    if (idx > list.size / 2) {
        start_at = list.tail;
        count = list.size - idx - 1;
    }

    node_t *curr = list_next(start_at, NULL);
    node_t *prev = start_at;

    for (size_t i = 0; i < count; i++) {
        node_t *next_link = list_next(curr, prev);
        prev = curr;
        curr = next_link;
    }
    return curr->value;
}

/* Changes the value at a selected index. */
bool list_set(list_t *list, size_t idx, list_val_t value) {
    size_t count = idx;
    node_t *start_at = list->head;

    if (idx > list->size) {
        return false;
    }

    /* Start at the end if setting a value closer to the end. */
    if (idx > list->size / 2) {
        count = list->size - idx - 1;
        start_at = list->tail;
    }

    node_t *curr = list_next(start_at, NULL);
    node_t *prev = start_at;

    for (size_t i = 0; i < count; i++) {
        node_t *next_node = list_next(curr, prev);
        prev = curr;
        curr = next_node;
    }

    curr->value = value;

    return true;
}

/* Provides the size of the list. */
size_t list_size(list_t list) {
    return list.size;
}

/* Remove an item from the list. */
size_t list_remove(list_t *list, list_val_t value) {
    size_t idx = list_find(*list, value);
    if (idx > list->size) {
        return idx;
    }

    list_delete(list, idx);

    return idx;
}

/* 
 * Provides the index of value in the list. Returns -1 if value is not in the
 * list.
 */
ssize_t list_find(list_t list, list_val_t value) {
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
bool list_contains(list_t list, list_val_t value) {
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
