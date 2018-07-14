#include <stdio.h>

#include "list.h"

int main(void) {

    list_t *list = list_create();
    printf("Get 0?: %d\n", list_get(*list, 0));
    for (int i = 1; i <= 10; i++) {
        list_append(list, i);
    }

    for (int i = 0; i < 10; i++) {
        int32_t val = list_get(*list, i);
        printf("%d: %d\n", i, val);
    }

    for (int i = 9; i >= 0; i--) {
        int32_t val = list_delete(list, i);
        printf("%d: %d\n", i, val);
    }

    printf("empty?: %d\n", list_is_empty(*list));

    printf("Get 0?: %d\n", list_get(*list, 0));

    printf("Destroying list.\n");

    list_destroy(list);

    return 0;
}
