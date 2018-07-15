#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <check.h>

#include "list.h"

START_TEST(CREATE_LIST)
{
    list_t *list = list_create();
    ck_assert(list);

    ck_assert(list->head->link == list->tail);
    ck_assert(list->tail->link == list->head);
    ck_assert(list->size == 0);
}
END_TEST

START_TEST(ADD_TO_LIST)
{
    list_t *list = list_create();

    ck_assert(!list_insert(list, 10, 1));
    ck_assert(!list_insert(list, -1, 1));

    list_val_t value = 1234;

    ck_assert(list_insert(list, 0, value));
    ck_assert(list_get(*list, 0) == value);

    /* Access node */
    node_t *added_node = (node_t *)((intptr_t)list->head->link ^ (intptr_t)NULL);
    ck_assert(added_node);
    ck_assert(added_node->value == value);
    ck_assert((intptr_t)added_node->link == ((intptr_t)list->head ^ (intptr_t)list->tail));
}
END_TEST

void tests (Suite *s) {
    TCase *tests = tcase_create("tests");
    tcase_add_test(tests, CREATE_LIST);
    tcase_add_test(tests, ADD_TO_LIST);
    suite_add_tcase(s, tests);
}
