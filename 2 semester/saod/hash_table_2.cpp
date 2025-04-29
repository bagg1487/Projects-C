#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

// === Линейное пробирование с проверкой уникальности ===
int linear_probing_chars(char A[], char table[], int n, int m) {
    int collisions = 0;
    unordered_set<char> unique_chars; // Для отслеживания уникальных символов
    
    for (int i = 0; i < n; i++) {
        // Пропускаем дубликаты
        if (unique_chars.count(A[i]) > 0) {
            continue;
        }
        unique_chars.insert(A[i]);
        
        int h = A[i] % m;
        int j = 0;
        while (j < m) {
            int idx = (h + j) % m;
            if (table[idx] == 0) {
                table[idx] = A[i];
                break;
            }
            // Считаем коллизию только если символы разные
            if (table[idx] != A[i]) {
                collisions++;
            }
            j++;
        }
    }
    return collisions;
}

// === Квадратичное пробирование с проверкой уникальности ===
int quadratic_probing_chars(char A[], char table[], int n, int m) {
    int collisions = 0;
    unordered_set<char> unique_chars; // Для отслеживания уникальных символов
    
    for (int i = 0; i < n; i++) {
        // Пропускаем дубликаты
        if (unique_chars.count(A[i]) > 0) {
            continue;
        }
        unique_chars.insert(A[i]);
        
        int h = A[i] % m;
        int j = 1;
        int attempt = 0;
        while (attempt < m) {
            if (table[h] == 0) {
                table[h] = A[i];
                break;
            }
            // Считаем коллизию только если символы разные
            if (table[h] != A[i]) {
                collisions++;
            }
            h = (h + j * j) % m;
            j++;
            attempt++;
        }
    }
    return collisions;
}

// === Красивая печать таблицы символов ===
void PrintSymbolTableFormatted(const char table[], int size, const string& title) {
    cout << "\n" << title << ":\n";
    const int cols = 5; // Уменьшил количество колонок для лучшей читаемости

    for (int row = 0; row < (size + cols - 1) / cols; row++) {
        int start = row * cols;
        int end = min(start + cols, size);

        // Индексы
        cout << "Индексы: ";
        for (int i = start; i < end; i++) {
            cout << setw(4) << i;
        }
        cout << "\nСимволы: ";
        for (int i = start; i < end; i++) {
            if (table[i] == 0)
                cout << setw(4) << '.';
            else
                cout << setw(4) << table[i];
        }
        cout << "\n";
    }
    cout << "\n";
}

// === Генерация случайного текста без дубликатов ===
void generate_unique_chars(char A[], int n) {
    unordered_set<char> used_chars;
    const string charset = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    
    for (int i = 0; i < n; ) {
        char c = charset[rand() % charset.size()];
        if (used_chars.count(c) == 0) {
            A[i] = c;
            used_chars.insert(c);
            i++;
        }
    }
    A[n] = '\0';
}

// === ЗАДАНИЕ 2 ===
void task2() {
    const int n = 20; // Количество уникальных символов
    const int m = 23; // Размер хеш-таблицы (простое число)
    
    char linear_table[m] = {0};
    char quadratic_table[m] = {0};
    char A[n+1];

    srand(time(NULL));
    generate_unique_chars(A, n); // Генерируем текст без дубликатов

    cout << "ХЕШИРОВАНИЕ УНИКАЛЬНЫХ СИМВОЛОВ (m = " << m << ") \n";
    


    int linear_coll = linear_probing_chars(A, linear_table, n, m);
    int quad_coll = quadratic_probing_chars(A, quadratic_table, n, m);

    PrintSymbolTableFormatted(linear_table, m, "Хеш-таблица (линейные пробы)");
    PrintSymbolTableFormatted(quadratic_table, m, "Хеш-таблица (квадратичные пробы)");

}

void task3() {
    vector<int> table_sizes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    const int n = 23;

    cout << " СРАВНЕНИЕ КОЛЛИЗИЙ (n = " << n << ")\n";
    
    cout << "│ Размер таблицы │ Элементов │ Линейные   │ Квадратичные   │\n";
    cout << "│       (m)      │   (n)     │ коллизии   │   коллизии     │\n";
    

    for (int m : table_sizes) {
        char A[n];
        char linear_table[m];
        char quadratic_table[m];
        fill_n(linear_table, m, 0);
        fill_n(quadratic_table, m, 0);
        

        int linear_coll = linear_probing_chars(A, linear_table, n, m);
        int quad_coll = quadratic_probing_chars(A, quadratic_table, n, m);
        

        cout << "│ " << setw(14) << m << " │ " << setw(9) << n
             << " │ " << setw(10) << linear_coll
             << " │ " << setw(14) << quad_coll << " │\n";
    }

}
// === MAIN ===
int main() {
    task2();
    task3();
    return 0;
}
