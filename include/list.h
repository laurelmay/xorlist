/*
 * Header file for xorlist.
 *
 * This file is licensed under the terms of the MIT License
 */
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

/* Adjust this typedef to change the types of values stored in the list. */
typedef int32_t list_val_t;

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
bool       list_add(list_t *, size_t, list_val_t);
bool       list_append(list_t *, list_val_t);
bool       list_prepend(list_t *, list_val_t);
bool       list_is_empty(list_t);
list_val_t list_delete(list_t *, size_t);
list_val_t list_get(list_t, size_t);
size_t     list_size(list_t);
ssize_t    list_find(list_t, list_val_t);
bool       list_contains(list_t, list_val_t);
