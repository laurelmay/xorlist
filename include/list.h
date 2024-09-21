/**
 * @file list.h
 * @author Laurel May (laurel@laurelmay.me)
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __LIST_H
#define __LIST_H
/*
 * Header file for xorlist.
 *
 * This file is licensed under the terms of the MIT License
 */
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * This list stores pointers. It is the responsibility of the caller to ensure
 * the pointers stored in the list are valid.
 */
typedef void *list_val_t;

/* Exported structs */

/**
 * @brief A node in the list
 *
 * This is an opaque type. Other than the head and tail of the list, it should
 * be very difficult to a reference to a node_t.
 */
typedef struct node node_t;

/**
 * @brief The XOR Linked List
 *
 * The list tracks a head, a tail, and the size of the list. The head and tail
 * will remain "static" for the life of the list.
 */
typedef struct
{
    /**
     * The head of the list. This is not a typical node and does not have
     * a value associated with it.
     */
    node_t *head;
    /**
     * The tail of the list. This is not a typical node and does not have
     * a value associated with it.
     */
    node_t *tail;
    /**
     * The number of elements stored in the list.
     */
    size_t size;
} list_t;

/**
 * @brief A function to tear-down elements in a list
 * 
 * This should accept a \ref list_val_t and properly teardown/destruct the
 * item, including passing it to `free(void *)` if needed.
 * 
 */
typedef void (*element_destructor)(list_val_t);

/* Exported list functions */
list_t *list_create(void);
void list_destroy(list_t *, element_destructor);
int list_insert(list_t *, size_t, list_val_t);
int list_append(list_t *, list_val_t);
int list_enqueue(list_t *, list_val_t);
int list_prepend(list_t *, list_val_t);
int list_push(list_t *, list_val_t);
bool list_is_empty(list_t);
list_val_t list_delete(list_t *, size_t);
ssize_t list_remove(list_t *, list_val_t);
list_val_t list_pop(list_t *);
list_val_t list_dequeue(list_t *);
list_val_t list_get(list_t, size_t);
list_val_t list_peek(list_t);
list_val_t list_set(list_t *, size_t, list_val_t);
size_t list_size(list_t);
ssize_t list_find(list_t, list_val_t);
bool list_contains(list_t, list_val_t);
void list_reverse(list_t *);

#endif
