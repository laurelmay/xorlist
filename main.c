#include <stdlib.h>
#include <stdio.h>

#include "list.h"

int main(void) {

    list_t *list = list_create();
    printf("Get idx 0?: %d\n", list_get(*list, 0));

    printf("\n== Add to list ==\n");
    for (int i = 1; i <= 10; i++) {
        list_append(list, i);
    }

    list_add(list, 5, 999);

    printf("\n== Get each item==\n");
    for (int i = 0; i < 11; i++) {
        int32_t val = list_get(*list, i);
        printf("%d: %d\n", i, val);
    }

    printf("\n== Find each item==\n");
    for (int i = 1; i <= 10; i++) {
        size_t loc = list_find(*list, i);
        printf("%ld @ %d\n", loc, i);
    }

    printf("\n== Delete each item==\n");
    for (int i = 10; i >= 0; i--) {
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
