#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <check.h>

#include "list.h"

START_TEST(LIST_CREATE)
{
    list_t *list = list_create();
    ck_assert(list);

    ck_assert(list->head->link == list->tail);
    ck_assert(list->tail->link == list->head);
    ck_assert(list->size == 0);

    list_destroy(list);
}
END_TEST

START_TEST(LIST_INSERT)
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
    
    list_destroy(list);
}
END_TEST


START_TEST(LIST_APPEND)
{
    list_t *list = list_create();

    ck_assert(list_append(list, 1));
    ck_assert(list_get(*list, 0) == 1);

    list_destroy(list);
}
END_TEST


START_TEST(LIST_PREPEND)
{
    list_t *list = list_create();
    ck_assert(list_prepend(list, 2));
    ck_assert(list_get(*list, 0) == 2);
    ck_assert(list_prepend(list, 1));
    ck_assert(list_get(*list, 0) == 1);

    list_destroy(list);
}
END_TEST


START_TEST(LIST_EMPTY)
{
    list_t *list = list_create();

    ck_assert(list_is_empty(*list));

    list_append(list, 1);

    ck_assert(!list_is_empty(*list));

    list_delete(list, 0);

    ck_assert(list_is_empty(*list));

    list_destroy(list);

}
END_TEST


START_TEST(LIST_DELETE)
{
    list_t *list = list_create();
    ck_assert(list->size == 0);
    list_append(list, 1);
    ck_assert(list->size == 1);
    list_delete(list, 0);
    ck_assert(list->size == 0);
    list_destroy(list);
}
END_TEST


START_TEST(LIST_GET)
{
    list_t *list = list_create();
    ck_assert(list_get(*list, 0) == -1);

    for (int i = 0; i < 10; i++) {
        list_append(list, i);
    }

    for (int i = 0; i < 10; i++) {
        ck_assert(list_get(*list, i) == i);
    }

    list_destroy(list);
}
END_TEST


START_TEST(LIST_SET)
{
    list_t *list = list_create();

    for (int i = 0; i < 10; i++) {
        list_append(list, i);
    }

    for (int i = 0; i < 10; i++) {
        list_set(list, i, i * 2);
    }

    for (int i = 0; i < 10; i++) {
        ck_assert(list_get(*list, i) == i * 2);
    }

    list_destroy(list);
}
END_TEST

START_TEST(LIST_SIZE) {
    list_t *list = list_create();

    size_t items = 0;

    for (int i = 0; i < 50; i++) {
        if (list_append(list, i)) {
            items++;
        }
    }

    ck_assert(list_size(*list) == items);

    for (int i = 0; i < 20; i++) {
        list_delete(list, 0);
        items--;
    }

    ck_assert(list_size(*list) == items);

    list_destroy(list);
}
END_TEST


START_TEST(LIST_REMOVE)
{
    list_t *list = list_create();

    for (int i = 0; i < 10; i++) {
        list_append(list, (i + 2) * 2);
    }

    ck_assert(list_size(*list) == 10);

    for (int i = 0; i < 10; i++) {
        int val = (i + 2) * 2;
        size_t idx = list_remove(list, val);
        ck_assert(idx == 0);
    }

    ck_assert(list_size(*list) == 0);

    list_destroy(list);
}
END_TEST


START_TEST(LIST_FIND)
{
    list_t *list = list_create();

    for (int i = 0; i < 10; i++) {
        list_append(list, (i + 2) * 2);
    }

    for (int i = 0; i < 10; i++) {
        int val = (i + 2) * 2;
        ssize_t idx = list_find(*list, val);
        ck_assert(idx == i);
    }

    list_destroy(list);
}
END_TEST


START_TEST(LIST_CONTAINS)
{
    list_t *list = list_create();

    for (int i = 0; i < 10; i++) {
        list_append(list, (i + 2) * 2);
    }

    for (int i = 0; i < 10; i++) {
        int val = (i + 2) * 2;
        ck_assert(list_contains(*list, val));
    }

    list_destroy(list);
}
END_TEST


void tests (Suite *s) {
    TCase *tests = tcase_create("tests");
    tcase_add_test(tests, LIST_CREATE);
    tcase_add_test(tests, LIST_INSERT);
    tcase_add_test(tests, LIST_APPEND);
    tcase_add_test(tests, LIST_PREPEND);
    tcase_add_test(tests, LIST_EMPTY);
    tcase_add_test(tests, LIST_DELETE);
    tcase_add_test(tests, LIST_GET);
    tcase_add_test(tests, LIST_SET);
    tcase_add_test(tests, LIST_SIZE);
    tcase_add_test(tests, LIST_REMOVE);
    tcase_add_test(tests, LIST_FIND);
    tcase_add_test(tests, LIST_CONTAINS);
    suite_add_tcase(s, tests);
}
