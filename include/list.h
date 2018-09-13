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

/*
 * This list stores pointers. It is the responsibility of the caller to ensure
 * the pointers stored in the list are valid.
 */
typedef void* list_val_t;


/* Exported structs */

typedef struct node {
    /*
     * Stores the address of the next item in the list and the previous
     * item in the list XORed (&next ^ &prev).
     */
    struct node* link;
    list_val_t value;
} node_t;


typedef struct {
    node_t *head;
    node_t *tail;
    size_t size;
} list_t;


/* Exported list functions */
list_t    *list_create();
void       list_destroy(list_t *);
bool       list_insert(list_t *, size_t, list_val_t);
bool       list_append(list_t *, list_val_t);
bool       list_enqueue(list_t *, list_val_t);
bool       list_prepend(list_t *, list_val_t);
bool       list_push(list_t *, list_val_t);
bool       list_is_empty(list_t);
list_val_t list_delete(list_t *, size_t);
ssize_t    list_remove(list_t *, list_val_t);
list_val_t list_pop(list_t *);
list_val_t list_dequeue(list_t *);
list_val_t list_get(list_t, size_t);
list_val_t list_peek(list_t);
bool       list_set(list_t *, size_t, list_val_t);
size_t     list_size(list_t);
ssize_t    list_find(list_t, list_val_t);
bool       list_contains(list_t, list_val_t);

#endif
