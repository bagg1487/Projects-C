#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define N 100

typedef struct Node {
    int key;
    int weight;
    struct Node* left;
    struct Node* right;
} Node;

// Функции для работы с деревом
Node* create_node(int key, int weight) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->weight = weight;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree(Node* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// Точный алгоритм построения оптимального дерева поиска
Node* build_optimal_bst(int keys[], int weights[], int n, 
                        int AW[][N+1], int AP[][N+1], int R[][N+1]) {
    // Инициализация матриц
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            AW[i][j] = 0;
            AP[i][j] = 0;
            R[i][j] = -1;
        }
    }
    
    // Заполнение диагоналей
    for (int i = 0; i < n; i++) {
        AW[i][i] = weights[i];
        AP[i][i] = weights[i];
        R[i][i] = i;
    }
    
    // Основной алгоритм
    for (int length = 1; length <= n; length++) {
        for (int i = 0; i <= n - length; i++) {
            int j = i + length;
            AW[i][j] = AW[i][j-1] + weights[j-1];
            
            int min_cost = INT_MAX;
            int best_root = -1;
            
            // Поиск оптимального корня (алгоритм Кнута)
            int start = (i < j-1) ? R[i][j-1] : i;
            int end = (i < j-1) ? R[i+1][j] : i;
            
            for (int k = start; k <= end && k < j; k++) {
                int cost = (k > i ? AP[i][k] : 0) + 
                          (k < j-1 ? AP[k+1][j] : 0);
                
                if (cost < min_cost) {
                    min_cost = cost;
                    best_root = k;
                }
            }
            
            AP[i][j] = min_cost + AW[i][j];
            R[i][j] = best_root;
        }
    }
    
    // Рекурсивное построение дерева
    Node* build_tree_rec(int i, int j, int keys[]) {
        if (i >= j) return NULL;
        
        int root_index = R[i][j];
        Node* root = create_node(keys[root_index], weights[root_index]);
        
        root->left = build_tree_rec(i, root_index, keys);
        root->right = build_tree_rec(root_index + 1, j, keys);
        
        return root;
    }
    
    return build_tree_rec(0, n, keys);
}

// Приближенный алгоритм A1: выбор корня с максимальным весом
Node* build_approx_bst_a1(int keys[], int weights[], int start, int end) {
    if (start > end) return NULL;
    
    // Находим вершину с максимальным весом в текущем диапазоне
    int max_weight_index = start;
    for (int i = start + 1; i <= end; i++) {
        if (weights[i] > weights[max_weight_index]) {
            max_weight_index = i;
        }
    }
    
    Node* root = create_node(keys[max_weight_index], weights[max_weight_index]);
    
    // Рекурсивно строим левое и правое поддеревья
    root->left = build_approx_bst_a1(keys, weights, start, max_weight_index - 1);
    root->right = build_approx_bst_a1(keys, weights, max_weight_index + 1, end);
    
    return root;
}

// Приближенный алгоритм A2: выбор корня, который делит вес пополам
Node* build_approx_bst_a2(int keys[], int weights[], int start, int end) {
    if (start > end) return NULL;
    
    int total_weight = 0;
    for (int i = start; i <= end; i++) {
        total_weight += weights[i];
    }
    
    // Находим корень, который делит суммарный вес примерно пополам
    int half_weight = total_weight / 2;
    int current_weight = 0;
    int root_index = start;
    
    for (int i = start; i <= end; i++) {
        current_weight += weights[i];
        if (current_weight >= half_weight) {
            root_index = i;
            break;
        }
    }
    
    Node* root = create_node(keys[root_index], weights[root_index]);
    
    // Рекурсивно строим левое и правое поддеревья
    root->left = build_approx_bst_a2(keys, weights, start, root_index - 1);
    root->right = build_approx_bst_a2(keys, weights, root_index + 1, end);
    
    return root;
}

// Обход дерева слева направо (in-order)
void in_order_traversal(Node* root) {
    if (root == NULL) return;
    in_order_traversal(root->left);
    printf("%d(%d) ", root->key, root->weight);
    in_order_traversal(root->right);
}

// Вычисление характеристик дерева
void calculate_tree_stats(Node* root, int* size, int* control_sum, int* height, 
                         double* weighted_height, int current_height) {
    if (root == NULL) return;
    
    (*size)++;
    *control_sum += root->key * root->weight;
    
    if (current_height > *height) {
        *height = current_height;
    }
    
    *weighted_height += root->weight * current_height;
    
    calculate_tree_stats(root->left, size, control_sum, height, weighted_height, current_height + 1);
    calculate_tree_stats(root->right, size, control_sum, height, weighted_height, current_height + 1);
}

// Вывод матриц (для демонстрации)
void print_matrix(int matrix[][N+1], int n, const char* name, int show_all) {
    printf("\n%s:\n", name);
    int limit = show_all ? n : 10;
    for (int i = 0; i <= limit; i++) {
        for (int j = 0; j <= limit; j++) {
            if (j >= i) {
                printf("%4d ", matrix[i][j]);
            } else {
                printf("     ");
            }
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    int keys[N];
    int weights[N];
    int original_weights[N]; // Сохраняем оригинальные веса для всех алгоритмов
    
    // Инициализация последовательности вершин
    printf("Генерация %d вершин с весами от 1 до 100...\n", N);
    for (int i = 0; i < N; i++) {
        keys[i] = i + 1;
        weights[i] = rand() % 100 + 1;
        original_weights[i] = weights[i]; // Сохраняем для всех алгоритмов
    }
    
    // Матрицы для точного алгоритма
    int AW[N+1][N+1];  // Матрица весов
    int AP[N+1][N+1];  // Матрица взвешенных высот
    int R[N+1][N+1];   // Матрица корней
    
    printf("\n=== ПОСТРОЕНИЕ ДЕРЕВА ОПТИМАЛЬНОГО ПОИСКА (ДОП) ===\n");
    Node* optimal_root = build_optimal_bst(keys, weights, N, AW, AP, R);
    
    // Вывод матриц (первые 10 строк/столбцов для наглядности)
    print_matrix(AW, N, "Матрица весов AW (первые 10)", 0);
    print_matrix(AP, N, "Матрица взвешенных высот AP (первые 10)", 0);
    print_matrix(R, N, "Матрица корней R (первые 10)", 0);
    
    printf("\nОбход ДОП слева направо (первые 20 элементов):\n");
    in_order_traversal(optimal_root);
    printf("\n");
    
    // Вычисление характеристик ДОП
    int optimal_size = 0, optimal_control_sum = 0, optimal_height = 0;
    double optimal_weighted_height = 0.0;
    calculate_tree_stats(optimal_root, &optimal_size, &optimal_control_sum, 
                        &optimal_height, &optimal_weighted_height, 1);
    optimal_weighted_height /= AW[0][N];
    
    printf("\n=== ПРИБЛИЖЕННЫЙ АЛГОРИТМ A1 (максимальный вес) ===\n");
    // Восстанавливаем оригинальные веса для A1
    for (int i = 0; i < N; i++) {
        weights[i] = original_weights[i];
    }
    Node* a1_root = build_approx_bst_a1(keys, weights, 0, N-1);
    
    printf("Обход дерева A1 слева направо (первые 20 элементов):\n");
    in_order_traversal(a1_root);
    printf("\n");
    
    // Вычисление характеристик A1
    int a1_size = 0, a1_control_sum = 0, a1_height = 0;
    double a1_weighted_height = 0.0;
    calculate_tree_stats(a1_root, &a1_size, &a1_control_sum, 
                       &a1_height, &a1_weighted_height, 1);
    a1_weighted_height /= AW[0][N]; // Используем тот же суммарный вес
    
    printf("\n=== ПРИБЛИЖЕННЫЙ АЛГОРИТМ A2 (деление веса пополам) ===\n");
    // Восстанавливаем оригинальные веса для A2
    for (int i = 0; i < N; i++) {
        weights[i] = original_weights[i];
    }
    Node* a2_root = build_approx_bst_a2(keys, weights, 0, N-1);
    
    printf("Обход дерева A2 слева направо (первые 20 элементов):\n");
    in_order_traversal(a2_root);
    printf("\n");
    
    // Вычисление характеристик A2
    int a2_size = 0, a2_control_sum = 0, a2_height = 0;
    double a2_weighted_height = 0.0;
    calculate_tree_stats(a2_root, &a2_size, &a2_control_sum, 
                       &a2_height, &a2_weighted_height, 1);
    a2_weighted_height /= AW[0][N]; // Используем тот же суммарный вес
    
    // Вывод итоговой таблицы
    printf("\n====================================================================\n");
    printf("n=%d  Размер  Контр.Сумма  Высота  Средневзвеш.высота\n", N);
    printf("====================================================================\n");
    printf("ДОП  %6d  %11d  %6d  %19.2f\n", 
           optimal_size, optimal_control_sum, optimal_height, optimal_weighted_height);
    printf("A1   %6d  %11d  %6d  %19.2f\n", 
           a1_size, a1_control_sum, a1_height, a1_weighted_height);
    printf("A2   %6d  %11d  %6d  %19.2f\n", 
           a2_size, a2_control_sum, a2_height, a2_weighted_height);
    printf("====================================================================\n");
    
    // Сравнение эффективности
    printf("\nСРАВНЕНИЕ ЭФФЕКТИВНОСТИ:\n");
    printf("Средневзвешенная высота ДОП: %.2f\n", optimal_weighted_height);
    printf("Средневзвешенная высота A1:  %.2f (%.2f%% хуже)\n", 
           a1_weighted_height, (a1_weighted_height/optimal_weighted_height - 1) * 100);
    printf("Средневзвешенная высота A2:  %.2f (%.2f%% хуже)\n", 
           a2_weighted_height, (a2_weighted_height/optimal_weighted_height - 1) * 100);
    
    // Проверка правильности точного алгоритма
    double calculated_weighted_height = (double)AP[0][N] / AW[0][N];
    printf("\nПроверка правильности точного алгоритма:\n");
    printf("AP[0,n]/AW[0,n] = %.6f\n", calculated_weighted_height);
    printf("Вычисленная средневзвешенная высота ДОП = %.6f\n", optimal_weighted_height);
    printf("Разница = %.6f\n", calculated_weighted_height - optimal_weighted_height);
    
    // Освобождение памяти
    free_tree(optimal_root);
    free_tree(a1_root);
    free_tree(a2_root);
    
    return 0;
}
