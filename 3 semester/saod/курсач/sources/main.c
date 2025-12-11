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
    printf("S = 1 - Метод сортировки: Вильямса-Флойда\n");
    printf("D = 1 - Тип дерева поиска: АВЛ-дерево\n");
    printf("E = 1 - Метод кодирования: Код Хаффмана\n");
    printf("===========================\n\n");
}

void paginated_view(Record** records, int count, const char* title) {
    int page_size = 20;
    int current_page = 0;
    int total_pages = (count + page_size - 1) / page_size;
    
    while (current_page < total_pages) {
        int start = current_page * page_size;
        int end = start + page_size;
        if (end > count) end = count;
        
        printf("\n%s - Страница %d/%d\n", title, current_page + 1, total_pages);
        print_records_table(records, start, end);
        
        if (current_page < total_pages - 1) {
            if (!confirm_continue()) break;
        }
        current_page++;
    }
}

void show_menu(int sorted, int has_search_results, int has_tree) {
    printf("\n=== ГЛАВНОЕ МЕНЮ ===\n");
    printf("1. Просмотр записей базы данных\n");
    if (!sorted) printf("2. Отсортировать базу данных\n");
    if (sorted) printf("3. Поиск по улице в отсортированной базе\n");
    if (has_search_results && !has_tree) printf("4. Построить дерево из результатов поиска\n");
    if (has_tree) printf("5. Поиск в дереве по ФИО\n");
    
    int huffman_choice;
    if (!sorted) {
        huffman_choice = has_tree ? 6 : (has_search_results ? 5 : 4);
    } else {
        huffman_choice = has_tree ? 6 : (has_search_results ? 5 : 4);
    }
    
    printf("%d. Кодирование файла\n", huffman_choice);
    printf("%d. Выход\n", huffman_choice + 1);
    printf("Выберите действие: ");
}

void run_huffman_encoding() {
    printf("\n=== НЕЗАВИСИМОЕ КОДИРОВАНИЕ ФАЙЛА ===\n");
    printf("Файл будет прочитан с нуля и полностью закодирован.\n\n");
    
    huffman_encode_file("testBase4.dat");
    
    printf("\nКодирование завершено.\n");
    printf("Создан файл: testBase4.dat.huffman\n");
}

int main() {
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    Record** records = NULL;
    int record_count = 0;
    int sorted = 0;
    QueueNode* search_results = NULL;
    TreeNode* search_tree = NULL;
    
    printf("=== КУРСОВАЯ РАБОТА. БАЗА ДАННЫХ 'НАСЕЛЕННЫЙ ПУНКТ' ===\n");
    show_variant_info();
    
    printf("1. Загрузка базы данных...\n");
    record_count = load_database("testBase4.dat", &records);
    if (record_count == 0) {
        printf("Ошибка загрузки базы данных!\n");
        return 1;
    }
    
    printf("Успешно загружено %d записей.\n\n", record_count);
    
    int choice;
    do {
        show_menu(sorted, search_results != NULL, search_tree != NULL);
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Неверный ввод!\n");
            continue;
        }
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
                    break;
                case 3:
                    if (sorted) {
                        printf("Введите первые 3 буквы названия улицы: ");
                        char search[10];
                        fgets(search, sizeof(search), stdin);
                        search[strcspn(search, "\n")] = '\0';
                        
                        if (strlen(search) >= 3) {
                            if (search_results) free_queue(search_results);
                            search_results = binary_search_by_street(records, record_count, search);
                            if (search_results) {
                                char title[100];
                                snprintf(title, sizeof(title), "Результаты поиска по улице на '%s'", search);
                                print_queue_table(search_results, title);
                            } else {
                                printf("Записи не найдены.\n");
                            }
                        }
                    } else {
                        printf("Сначала выполните сортировку данных!\n");
                    }
                    break;
                case 4:
                    if (search_results && !search_tree) {
                        printf("\nПостроение АВЛ-дерева из результатов поиска...\n");
                        search_tree = create_tree_from_queue(search_results);
                        print_tree_table(search_tree);
                    } else if (search_tree) {
                        printf("Дерево уже построено!\n");
                    } else {
                        run_huffman_encoding();
                    }
                    break;
                case 5:
                    if (search_tree) {
                        printf("Введите первые 3 буквы фамилии для поиска в дереве: ");
                        char search_fio[10];
                        fgets(search_fio, sizeof(search_fio), stdin);
                        search_fio[strcspn(search_fio, "\n")] = '\0';
                        
                        if (strlen(search_fio) >= 3) {
                            QueueNode* found_queue = search_tree_by_fio_prefix(search_tree, search_fio);
                            if (found_queue) {
                                char title[100];
                                snprintf(title, sizeof(title), "Результаты поиска в дереве по фамилии на '%s'", search_fio);
                                print_queue_table(found_queue, title);
                                free_queue(found_queue);
                            } else {
                                printf("Записи не найдены в дереве.\n");
                            }
                        } else {
                            printf("Введите минимум 3 буквы фамилии!\n");
                        }
                    } else {
                        run_huffman_encoding();
                    }
                    break;
                case 6:
                    run_huffman_encoding();
                    break;
                case 7:
                    printf("Выход...\n");
                    break;
                default:
                    printf("Неверный выбор!\n");
            }
        } else {
            switch (choice) {
                case 1:
                    paginated_view(records, record_count, "Отсортированные записи");
                    break;
                case 2:
                    printf("Введите первые 3 буквы названия улицы: ");
                    char search[10];
                    fgets(search, sizeof(search), stdin);
                    search[strcspn(search, "\n")] = '\0';
                    
                    if (strlen(search) >= 3) {
                        if (search_results) free_queue(search_results);
                        search_results = binary_search_by_street(records, record_count, search);
                        if (search_results) {
                            char title[100];
                            snprintf(title, sizeof(title), "Результаты поиска по улице на '%s'", search);
                            print_queue_table(search_results, title);
                        } else {
                            printf("Записи не найдены.\n");
                        }
                    }
                    break;
                case 3:
                    if (search_results && !search_tree) {
                        printf("\nПостроение АВЛ-дерева из результатов поиска...\n");
                        search_tree = create_tree_from_queue(search_results);
                        print_tree_table(search_tree);
                    } else if (search_tree) {
                        printf("Дерево уже построено!\n");
                    } else {
                        run_huffman_encoding();
                    }
                    break;
                case 4:
                    if (search_tree) {
                        printf("Введите первые 3 буквы фамилии для поиска в дереве: ");
                        char search_fio[10];
                        fgets(search_fio, sizeof(search_fio), stdin);
                        search_fio[strcspn(search_fio, "\n")] = '\0';
                        
                        if (strlen(search_fio) >= 3) {
                            QueueNode* found_queue = search_tree_by_fio_prefix(search_tree, search_fio);
                            if (found_queue) {
                                char title[100];
                                snprintf(title, sizeof(title), "Результаты поиска в дереве по фамилии на '%s'", search_fio);
                                print_queue_table(found_queue, title);
                                free_queue(found_queue);
                            } else {
                                printf("Записи не найдены в дереве.\n");
                            }
                        } else {
                            printf("Введите минимум 3 буквы фамилии!\n");
                        }
                    } else {
                        run_huffman_encoding();
                    }
                    break;
                case 5:
                    run_huffman_encoding();
                    break;
                case 6:
                    printf("Выход...\n");
                    break;
                default:
                    printf("Неверный выбор!\n");
            }
        }
    } while (choice != 7 && choice != 6);
            
    if (search_results) free_queue(search_results);
    if (search_tree) free_tree(search_tree);
    free_database(records, record_count);

    printf("\nПрограмма завершена.\n");
    return 0;
}