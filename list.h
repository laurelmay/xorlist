/*
 * Header file for xorlist.
 *
 * This file is licensed under the terms of the MIT License
 */
#include <stdlib.h>
#include <sys/types.h>

typedef int32_t list_val_t;

typedef struct node {
    struct node* link;
    list_val_t value;
} node_t;

typedef struct list {
    node_t *head;
    node_t *tail;
    size_t size;
} list_t;

list_t *create_list();
void destroy_list(list_t *);
bool add(list_t *, size_t, list_val_t);
bool append(list_t *, list_val_t);
bool prepend(list_t *, list_val_t);
bool is_empty(list_t);
list_val_t delete(list_t *, size_t);
list_val_t get(list_t, size_t);
size_t size(list_t);
ssize_t find(list_t, list_val_t);
bool contains(list_t, list_val_t);
