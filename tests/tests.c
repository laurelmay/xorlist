#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <check.h>

#include "list.h"

typedef struct data {
    int val;
} data_t;

static int destroy_count = 0;
void destroy_counter(list_val_t _) {
    destroy_count += 1;
}

START_TEST(LIST_CREATE)
{
    list_t *list = list_create();
    ck_assert(list);

    ck_assert(list->size == 0);

    list_destroy(list, nullptr);
}
END_TEST

START_TEST(LIST_DESTROY)
{
    list_t *list = list_create();
    ck_assert(list);

    size_t items = 10;
    for (size_t i = 0; i < items; i++) list_append(list, nullptr);

    destroy_count = 0;
    list_destroy(list, destroy_counter);
    ck_assert(destroy_count == items);
    destroy_count = 0;

}
END_TEST

START_TEST(LIST_DESTROY_FREE)
{
    list_t *list = list_create();
    ck_assert(list);

    size_t items = 10;
    for (size_t i = 0; i < items; i++) list_append(list, malloc(sizeof(int)));

    list_destroy(list, free);
}
END_TEST

START_TEST(LIST_INSERT)
{
    list_t *list = list_create();

    data_t one = { .val = 1 };

    ck_assert_msg(list_insert(list, 10, &one), "Adding at an invalid index fails");
    ck_assert_msg(list_insert(list, -1, &one), "Adding at a negative index fails");

    data_t dat = { .val = 1234 };
    list_val_t value = &dat;

    ck_assert_msg(!list_insert(list, 0, value), "Adding at a valid index (0) succeeds");
    ck_assert(list_get(*list, 0) == value);

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_INSERT_INDEX)
{
    list_t *list = list_create();
    data_t values[10];

    for (int i = 0; i < 10; i++) {
        values[i] = (data_t){ .val = i };
        list_append(list, values + i);
    }


    data_t four_hundred = { .val = 400 };
    data_t six_hundred = { .val = 600 };
    
    list_insert(list, 4, &four_hundred);
    list_insert(list, 6, &six_hundred);

    for (int i = 0; i < 4; i++) {
        ck_assert(list_get(*list, i) == values + i);
    }

    ck_assert(list_get(*list, 4) == &four_hundred);
    ck_assert(list_get(*list, 5) == values + 4);
    ck_assert(list_get(*list, 6) == &six_hundred);

    for (int i = 7; i < list_size(*list); i++) {
        ck_assert(list_get(*list, i) == values + (i - 2));
    }

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_APPEND)
{
    list_t *list = list_create();

    data_t one = { .val = 1 };

    ck_assert(!list_append(list, &one));
    ck_assert(list_get(*list, 0) == &one);

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_ENQUEUE)
{
    list_t *list = list_create();

    data_t one = { .val = 1 };
    ck_assert(!list_enqueue(list, &one));
    ck_assert(list_get(*list, 0) == &one);

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_PREPEND)
{
    list_t *list = list_create();
    data_t one = { .val = 1 };
    data_t two = { .val = 2 };
    ck_assert(!list_prepend(list, &two));
    ck_assert(list_get(*list, 0) == &two);
    ck_assert(!list_prepend(list, &one));
    ck_assert(list_get(*list, 0) == &one);

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_PUSH)
{
    list_t *list = list_create();
    data_t one = { .val = 1 };
    data_t two = { .val = 2 };
    ck_assert(!list_push(list, &two));
    ck_assert(list_get(*list, 0) == &two);
    ck_assert(!list_push(list, &one));
    ck_assert(list_get(*list, 0) == &one);

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_EMPTY)
{
    list_t *list = list_create();

    ck_assert(list_is_empty(*list));

    data_t one = { .val = 1 };
    list_append(list, &one);

    ck_assert(!list_is_empty(*list));

    list_delete(list, 0);

    ck_assert(list_is_empty(*list));

    list_destroy(list, nullptr);

}
END_TEST


START_TEST(LIST_DELETE)
{
    list_t *list = list_create();
    data_t one = { .val = 1 };
    ck_assert(list->size == 0);
    list_append(list, &one);
    ck_assert(list->size == 1);
    list_delete(list, 0);
    ck_assert(list->size == 0);
    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_DEQUEUE)
{
    list_t *list = list_create();
    ck_assert(list->size == 0);

    data_t values[5];
    for (int i = 0; i < 5; i++) {
        values[i] = (data_t){ .val = i };
        list_enqueue(list, values + i);
    }
    for (int i = 0; i < 5; i++) {
        ck_assert(list_dequeue(list) == values + i);
    }
    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_POP)
{
    list_t *list = list_create();
    ck_assert(list->size == 0);

    data_t values[5];
    for (int i = 0; i < 5; i++) {
        values[i] = (data_t) { .val = i };
        list_push(list, values + i);
    }
    for (int i = 0; i < 5; i++) {
        ck_assert(list_pop(list) == values + (4 - i));
    }
    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_GET)
{
    list_t *list = list_create();
    ck_assert(list_get(*list, 0) == nullptr);

    data_t values[10];
    for (int i = 0; i < 10; i++) {
        values[i] = (data_t) { .val = i };
        list_append(list, values + i);
    }

    for (int i = 0; i < 10; i++) {
        ck_assert(list_get(*list, i) == values + i);
    }

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_PEEK)
{
    list_t *list = list_create();
    ck_assert(list_peek(*list) == nullptr);

    for (int i = 0; i < 10; i++) {
        list_append(list, &(data_t) { .val = i });
    }

    for (int i = 0; i < 10; i++) {
        ck_assert(list_peek(*list) == list_pop(list));
    }

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_SET)
{
    list_t *list = list_create();

    data_t values[10];
    for (int i = 0; i < 10; i++) {
        values[i] = (data_t) { .val = i };
        list_append(list, values + i);
    }

    for (int i = 0; i < 10; i++) {
        list_set(list, i, values + 10 - i);
    }

    for (int i = 0; i < 10; i++) {
        ck_assert(list_get(*list, i) == values + 10 - i);
    }

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_SIZE) {
    list_t *list = list_create();

    size_t items = 0;

    for (int i = 0; i < 50; i++) {
        list_append(list, &(data_t) { .val = i });
        items++;
    }

    ck_assert(list_size(*list) == items);

    for (int i = 0; i < 20; i++) {
        list_delete(list, 0);
        items--;
    }

    ck_assert(list_size(*list) == items);

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_REMOVE)
{
    list_t *list = list_create();

    data_t values[50];
    for (int i = 0; i < 50; i++) {
        values[i] = (data_t) { .val = i };
    }

    for (int i = 0; i < 10; i++) {
        list_append(list, values + ((i + 2) * 2));
    }

    ck_assert(list_size(*list) == 10);

    for (int i = 0; i < 10; i++) {
        data_t *val = values + ((i + 2) * 2);
        ssize_t idx = list_remove(list, val);
        ck_assert(idx == 0);
    }

    ck_assert(list_size(*list) == 0);

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_FIND)
{
    list_t *list = list_create();

    data_t values[50];
    for (int i = 0; i < 50; i++) {
        values[i] = (data_t) { .val = i };
    }

    for (int i = 0; i < 10; i++) {
        list_append(list, values + ((i + 2) * 2));
    }

    for (int i = 0; i < 10; i++) {
        data_t *val = values + ((i + 2) * 2);
        ssize_t idx = list_find(*list, val);
        ck_assert(idx == i);
    }

    list_destroy(list, nullptr);
}
END_TEST


START_TEST(LIST_CONTAINS)
{
    list_t *list = list_create();

    data_t values[50];
    for (int i = 0; i < 50; i++) {
        values[i] = (data_t) { .val = i };
    }

    for (int i = 0; i < 10; i++) {
        list_append(list, values + ((i + 2) * 2));
    }

    for (int i = 0; i < 10; i++) {
        data_t *val = values + ((i + 2) * 2);
        ck_assert(list_contains(*list, val));
    }

    list_destroy(list, nullptr);
}
END_TEST

START_TEST(LIST_REVERSE)
{
    list_t *list = list_create();
    data_t values[50];
    for (int i = 0; i < 50; i++) {
        values[i] = (data_t) { .val = i };
        list_append(list, values + i);
    }
    list_reverse(list);
    for (int i = 49; i >= 0; i--) {
        ck_assert(((data_t *)(list_pop(list)))->val == i);
    }

}
END_TEST


void tests (Suite *s) {
    TCase *tests = tcase_create("tests");
    tcase_add_test(tests, LIST_CREATE);
    tcase_add_test(tests, LIST_DESTROY);
    tcase_add_test(tests, LIST_DESTROY_FREE);
    tcase_add_test(tests, LIST_INSERT);
    tcase_add_test(tests, LIST_INSERT_INDEX);
    tcase_add_test(tests, LIST_APPEND);
    tcase_add_test(tests, LIST_ENQUEUE);
    tcase_add_test(tests, LIST_PREPEND);
    tcase_add_test(tests, LIST_PUSH);
    tcase_add_test(tests, LIST_EMPTY);
    tcase_add_test(tests, LIST_DELETE);
    tcase_add_test(tests, LIST_GET);
    tcase_add_test(tests, LIST_PEEK);
    tcase_add_test(tests, LIST_SET);
    tcase_add_test(tests, LIST_SIZE);
    tcase_add_test(tests, LIST_REMOVE);
    tcase_add_test(tests, LIST_POP);
    tcase_add_test(tests, LIST_DEQUEUE);
    tcase_add_test(tests, LIST_FIND);
    tcase_add_test(tests, LIST_CONTAINS);
    tcase_add_test(tests, LIST_REVERSE);
    suite_add_tcase(s, tests);
}
