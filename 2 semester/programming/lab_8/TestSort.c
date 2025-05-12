#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"



static void test_SelectSort_basic(void **state) {
    (void) state; // Неиспользуемый параметр
    
    long long int arr[] = {5, 3, 1, 4, 2};
    long long int expected[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int operations = SelectSort(arr, n);
    
    // Проверяем, что массив отсортирован правильно
    for (int i = 0; i < n; i++) {
        assert_int_equal(arr[i], expected[i]);
    }
    
    // Проверяем, что количество операций неотрицательное
    assert_true(operations >= 0);
}

static void test_SelectSort2_basic(void **state) {
    (void) state;
    
    long long int arr[] = {10, 8, 6, 4, 2};
    long long int expected[] = {2, 4, 6, 8, 10};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int operations = SelectSort2(arr, n);
    
    // Проверяем сортировку
    for (int i = 0; i < n; i++) {
        assert_int_equal(arr[i], expected[i]);
    }
    
    // Проверяем, что количество операций неотрицательное
    assert_true(operations >= 0);
}

static void test_SelectSort_empty(void **state) {
    (void) state;
    
    long long int arr[] = {};
    int n = 0;
    
    int operations = SelectSort(arr, n);
    
    // Для пустого массива операций должно быть 0
    assert_int_equal(operations, 0);
}

static void test_SelectSort_already_sorted(void **state) {
    (void) state;
    
    long long int arr[] = {1, 2, 3, 4, 5};
    long long int expected[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int operations = SelectSort2(arr, n);
    
    // Массив уже отсортирован, проверяем что он не изменился
    for (int i = 0; i < n; i++) {
        assert_int_equal(arr[i], expected[i]);
    }
    
    // Проверяем, что количество операций корректное (сравнения будут, но обмены нет)
    assert_true(operations >= n*(n-1)/2); // Минимум C = n(n-1)/2 сравнений
}

static void test_SelectSort2_duplicates(void **state) {
    (void) state;
    
    long long int arr[] = {3, 1, 2, 1, 3};
    long long int expected[] = {1, 1, 2, 3, 3};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int operations = SelectSort2(arr, n);
    
    // Проверяем сортировку с дубликатами
    for (int i = 0; i < n; i++) {
        assert_int_equal(arr[i], expected[i]);
    }
    
    assert_true(operations >= 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_SelectSort_basic),
        cmocka_unit_test(test_SelectSort2_basic),
        cmocka_unit_test(test_SelectSort_empty),
        cmocka_unit_test(test_SelectSort_already_sorted),
        cmocka_unit_test(test_SelectSort2_duplicates),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
