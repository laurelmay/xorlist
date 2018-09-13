#include <stdlib.h>
#include <stdio.h>

#include "list.h"

int main(void)
{

    list_t *list = list_create();

    printf("== List Info ==\n");
    printf("&list = %p\n", (void *)list);
    printf("&head = %p\n", (void *)list->head);
    printf("&tail = %p\n", (void *)list->tail);

    printf("Get idx 0?: %p\n", list_get(*list, 0));

    printf("\n== Add to list ==\n");
    for (size_t i = 1; i <= 10; i++) {
        list_append(list, (void *)i);
    }

    list_insert(list, 4, (void *)400);
    list_insert(list, 6, (void *) 600);

    printf("\n== Get each item==\n");
    for (size_t i = 0; i < 12; i++) {
        void *val = list_get(*list, i);
        printf("%ld: %p\n", i, val);
    }

    printf("\n== Find each item==\n");
    for (size_t i = 1; i <= 10; i++) {
        size_t loc = list_find(*list, (void *)i);
        printf("%ld @ %ld\n", i, loc);
    }

    printf("%d @ %ld\n", 400, list_find(*list, (void *)400));
    printf("%d @ %ld\n", 600, list_find(*list, (void *)600));

    printf("\n== Delete each item==\n");
    for (size_t i = 0; i <= 11; i++) {
        void *val = list_delete(list, 0);
        printf("%ld: %p\n", i, val);
    }

    printf("\n");
    printf("empty?: %s\n", list_is_empty(*list) ? "Yes" : "No");

    printf("Get idx 0?: %p\n", list_get(*list, 0));

    printf("Destroying list.\n");

    list_destroy(list);

    return 0;
}
