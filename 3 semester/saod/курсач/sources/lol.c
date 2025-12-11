#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/database.h"
#include "../headers/sort.h"
#include "../headers/search.h"
#include "../headers/tree.h"
#include "../headers/huffman.h"

void show_variant_info() {
    printf("=== ПАРАМЕТРЫ ВАРИАНТА ===\n");
    printf("B = 4 - База данных 'Населенный пункт'\n");
    printf("C = 2 - Условие упорядочения: по названию улицы и номеру дома\n");
    printf("S = 1 - Метод сортировки: Вильямса-Флойда (пирамидальная)\n");
    printf("D = 1 - Тип дерева поиска: АВЛ-дерево\n");
    printf("E = 1 - Метод кодирования: Код Хаффмана\n");
    printf("===========================\n\n");
}

int ask_show_records() {
    printf("Показать записи? (y/n): ");
    char response;
    scanf(" %c", &response);
    getchar();
    return (response == 'y' || response == 'Y');
}

void paginated_view(Record** records, int count, const char* title) {
    if (!ask_show_records()) return;
    
    int page_size = 20;
    int current_page = 0;
    int total_pages = (count + page_size - 1) / page_size;
    
    printf("\n%s (по 20 на странице):\n", title);
    
    while (current_page < total_pages) {
        int start = current_page * page_size;
        int end = start + page_size;
        if (end > count) end = count;
        
        printf("\n=== Страница %d/%d ===\n", current_page + 1, total_pages);
        print_records(records, start, end, page_size);
        
        if (current_page < total_pages - 1) {
            printf("Показать следующую страницу? (y/n): ");
            char response;
            scanf(" %c", &response);
            getchar();
            
            if (response != 'y' && response != 'Y') {
                break;
            }
        }
        current_page++;
    }
}

void show_menu(int sorted) {
    printf("\n=== МЕНЮ ===\n");
    printf("1. Просмотр записей\n");
    if (!sorted) {
        printf("2. Отсортировать данные\n");
    }
    printf("%d. Выполнить поиск по улице\n", sorted ? 2 : 3);
    printf("%d. Построить дерево и выполнить поиск\n", sorted ? 3 : 4);
    printf("%d. Закодировать файл\n", sorted ? 4 : 5);
    printf("%d. Выход\n", sorted ? 5 : 6);
    printf("Выберите действие: ");
}

int main() {
    Record** records = NULL;
    int record_count = 0;
    int sorted = 0;
    QueueNode* found_queue = NULL;
    TreeNode* tree_root = NULL;
    
    printf("=== Курсовая работа. База данных 'Населенный пункт' ===\n");
    show_variant_info();
    
    printf("1. Загрузка базы данных...\n");
    record_count = load_database("testBase4.dat", &records);
    if (record_count == 0) {
        printf("Ошибка загрузки базы данных!\n");
        return 1;
    }
    printf("Загружено записей: %d\n", record_count);
    
    int choice;
    
    do {
        show_menu(sorted);
        scanf("%d", &choice);
        getchar();
        
        if (!sorted) {
            switch (choice) {
                case 1:
                    paginated_view(records, record_count, "Несортированные записи");
                    break;
                    
                case 2:
                    printf("\nСортировка по улице и номеру дома...\n");
                    heap_sort(records, record_count);
                    sorted = 1;
                    printf("Сортировка завершена.\n");
                    paginated_view(records, record_count, "Отсортированные записи");
                    break;
                    
                case 3:
                    printf("Сначала выполните сортировку!\n");
                    break;
                    
                case 4:
                    printf("Сначала выполните сортировку!\n");
                    break;
                    
                case 5:
                    printf("\nКодирование файла методом Хаффмана...\n");
                    build_huffman_tree("testBase4.dat");
                    break;
                    
                case 6:
                    printf("Выход...\n");
                    break;
                    
                default:
                    printf("Неверный выбор!\n");
                    break;
            }
        } else {
            switch (choice) {
                case 1:
                    paginated_view(records, record_count, "Отсортированные записи");
                    break;
                    
                case 2:
                    printf("\nБыстрый поиск по улице...\n");
                    printf("Введите название улицы для поиска: ");
                    char search_street[STREET_SIZE + 1];
                    fgets(search_street, STREET_SIZE, stdin);
                    search_street[strcspn(search_street, "\n")] = 0;
                    
                    char search_key[STREET_SIZE + 1];
                    strncpy(search_key, search_street, STREET_SIZE);
                    for (int i = strlen(search_key); i < STREET_SIZE; i++) {
                        search_key[i] = '_';
                    }
                    search_key[STREET_SIZE] = '\0';
                    
                    if (found_queue) free_queue(found_queue);
                    found_queue = binary_search(records, record_count, search_key);
                    
                    if (found_queue) {
                        printf("Найдены записи для улицы '%s'\n", search_street);
                        char title[100];
                        snprintf(title, sizeof(title), "Результаты поиска по улице '%s'", search_street);
                        print_queue_paginated(found_queue, title);
                    } else {
                        printf("Записи не найдены.\n");
                    }
                    break;
                    
                case 3:
                    if (!sorted) {
                        printf("Сначала выполните сортировку!\n");
                        break;
                    }
                    if (!found_queue) {
                        printf("Сначала выполните поиск по улице (пункт 2)!\n");
                        break;
                    }
                    
                    printf("\nПостроение AVL-дерева из найденных записей...\n");
                    if (tree_root) free_tree(tree_root);
                    tree_root = create_tree_from_queue(found_queue);
                    
                    // Считаем количество записей в очереди
                    int queue_count = 0;
                    QueueNode* temp = found_queue;
                    while (temp) {
                        queue_count++;
                        temp = temp->next;
                    }
                    
                    printf("Дерево построено (из %d записей очереди)\n", queue_count);
                    
                    if (ask_show_records()) {
                        printf("\nСодержимое дерева (упорядочено по ФИО):\n");
                        print_tree_inorder(tree_root);
                    }
                    
                    // Цикл поиска в дереве
                    int search_choice;
                    do {
                        printf("\n=== Поиск в дереве ===\n");
                        printf("Выберите поле для поиска:\n");
                        printf("1. ФИО\n");
                        printf("2. Дом\n");
                        printf("3. Квартира\n");
                        printf("4. Дата\n");
                        printf("5. Вернуться в главное меню\n");
                        printf("Выберите действие: ");
                        
                        scanf("%d", &search_choice);
                        getchar();
                        
                        if (search_choice >= 1 && search_choice <= 4) {
                            char search_value[100];
                            const char* field_name = "";
                            
                            switch (search_choice) {
                                case 1:
                                    printf("Введите ФИО для поиска: ");
                                    field_name = "fio";
                                    break;
                                case 2:
                                    printf("Введите номер дома для поиска: ");
                                    field_name = "house";
                                    break;
                                case 3:
                                    printf("Введите номер квартиры для поиска: ");
                                    field_name = "apartment";
                                    break;
                                case 4:
                                    printf("Введите дату для поиска (дд-мм-гг): ");
                                    field_name = "date";
                                    break;
                            }
                            
                            if (search_choice >= 1 && search_choice <= 4) {
                                fgets(search_value, sizeof(search_value), stdin);
                                search_value[strcspn(search_value, "\n")] = 0;
                                
                                TreeNode* found_in_tree = search_tree_by_field(tree_root, field_name, search_value);
                                if (found_in_tree) {
                                    printf("\nНайдена запись:\n");
                                    printf("  ФИО: %s\n", found_in_tree->data->fio);
                                    printf("  Улица: %s\n", found_in_tree->data->street);
                                    printf("  Дом: %d\n", found_in_tree->data->house);
                                    printf("  Квартира: %d\n", found_in_tree->data->apartment);
                                    printf("  Дата: %s\n", found_in_tree->data->date);
                                } else {
                                    printf("Запись не найдена.\n");
                                }
                            }
                        } else if (search_choice != 5) {
                            printf("Неверный выбор!\n");
                        }
                    } while (search_choice != 5);
                    break;
                        
                case 4:
                    printf("\nКодирование файла методом Хаффмана...\n");
                    build_huffman_tree("testBase4.dat");
                    break;
                    
                case 5:
                    printf("Выход...\n");
                    break;
                    
                default:
                    printf("Неверный выбор!\n");
                    break;
            }
        }
    } while ((!sorted && choice != 6) || (sorted && choice != 5));
    
    if (found_queue) free_queue(found_queue);
    if (tree_root) free_tree(tree_root);
    free_database(records, record_count);
    
    printf("\nПрограмма завершена.\n");
    return 0;
}