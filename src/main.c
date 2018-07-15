#include <stdlib.h>
#include <stdio.h>

#include "list.h"

int main(void) {

    list_t *list = list_create();

    printf("== List Info ==\n");
    printf("&list = %p\n", (void *)list);
    printf("&head = %p\n", (void *)list->head);
    printf("&tail = %p\n", (void *)list->tail);

    printf("Get idx 0?: %d\n", list_get(*list, 0));

    printf("\n== Add to list ==\n");
    for (int i = 1; i <= 10; i++) {
        list_append(list, i);
    }

    list_insert(list, 4, 400);
    list_insert(list, 6, 600);

    printf("\n== Get each item==\n");
    for (int i = 0; i < 12; i++) {
        int32_t val = list_get(*list, i);
        printf("%d: %d\n", i, val);
    }

    printf("\n== Find each item==\n");
    for (int i = 1; i <= 10; i++) {
        size_t loc = list_find(*list, i);
        printf("%d @ %ld\n", i, loc);
    }

    printf("%d @ %ld\n", 400, list_find(*list, 400));
    printf("%d @ %ld\n", 600, list_find(*list, 600));

    printf("\n== Delete each item==\n");
    for (int i = 11; i >= 0; i--) {
        int32_t val = list_delete(list, i);
        printf("%d: %d\n", i, val);
    }

    printf("\n");
    printf("empty?: %s\n", list_is_empty(*list) ? "Yes" : "No");

    printf("Get idx 0?: %d\n", list_get(*list, 0));

    printf("Destroying list.\n");

    list_destroy(list);

    return 0;
}
