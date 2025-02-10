#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Функция для сортировки выбором с подсчётом сравнений и пересылок
void selectionSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparisons)++; // Увеличиваем счётчик сравнений
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int temp = arr[min_idx];
            arr[min_idx] = arr[i];
            arr[i] = temp;
            (*swaps)++; // Увеличиваем счётчик пересылок
        }
    }
}

// Функция для генерации случайного массива
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // Случайные числа от 0 до 999
    }
}

int main() {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Открываем файл для записи данных
    FILE *dataFile = fopen("data.txt", "w");
    if (dataFile == NULL) {
        printf("Ошибка открытия файла!\n");
        return 1;
    }

    // Диапазон размеров массива
    int start_n = 10;
    int end_n = 1000;
    int step = 10;

    // Перебираем размеры массива
    for (int n = start_n; n <= end_n; n += step) {
        int arr[n];
        generateRandomArray(arr, n); // Генерация случайного массива

        int comparisons, swaps;
        selectionSort(arr, n, &comparisons, &swaps); // Сортировка

        // Запись данных в файл: n, Mф + Cф
        fprintf(dataFile, "%d %d\n", n, comparisons + swaps);
    }

    fclose(dataFile); // Закрываем файл

    // Используем GNUplot для построения графика
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        printf("Ошибка запуска GNUplot!\n");
        return 1;
    }

    // Команды для GNUplot
    fprintf(gnuplot, "set title 'Зависимость Mф + Cф от размера массива n'\n");
    fprintf(gnuplot, "set xlabel 'Размер массива (n)'\n");
    fprintf(gnuplot, "set ylabel 'Mф + Cф'\n");
    fprintf(gnuplot, "plot 'data.txt' with lines title 'Mф + Cф'\n");

    pclose(gnuplot); // Закрываем GNUplot

    return 0;
}