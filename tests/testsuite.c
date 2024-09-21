#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include <check.h>

extern void tests (Suite *s);

Suite * test_suite (void) {
    Suite *s = suite_create("Default");
    tests(s);
    return s;
}

void run_testsuite(void) {
    Suite *s = test_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    srunner_free(sr);
}

int main(void) {
    srand((unsigned) time(nullptr));
    run_testsuite();
    return EXIT_SUCCESS;
}

