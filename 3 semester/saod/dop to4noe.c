#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define N 100

typedef struct Node {
    int key;
    int weight;
    struct Node* left;
    struct Node* right;
} Node;

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

Node* build_optimal_bst(int keys[], int weights[], int n, 
                        int AW[][N+1], int AP[][N+1], int R[][N+1]) {

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            AW[i][j] = 0;
            AP[i][j] = 0;
            R[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++) {
        AW[i][i] = weights[i];
        AP[i][i] = weights[i];
        R[i][i] = i;
    }

    for (int length = 1; length <= n; length++) {
        for (int i = 0; i <= n - length; i++) {
            int j = i + length;
            AW[i][j] = AW[i][j-1] + weights[j-1];
            
            int min_cost = INT_MAX;
            int best_root = -1;
            
            int start = (length == 1) ? i : R[i][j-1];
            int end = (length == 1) ? i : R[i+1][j];
            
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

    Node* build_tree(int i, int j) {
        if (i >= j) return NULL;
        
        int root_index = R[i][j];
        Node* root = create_node(keys[root_index], weights[root_index]);
        
        root->left = build_tree(i, root_index);
        root->right = build_tree(root_index + 1, j);
        
        return root;
    }
    
    return build_tree(0, n);
}

void in_order_traversal(Node* root) {
    if (root == NULL) return;
    in_order_traversal(root->left);
    printf("%d(%d) ", root->key, root->weight);
    in_order_traversal(root->right);
}

void calculate_tree_stats(Node* root, int* size, int* sum, int* height, double* weighted_height, int current_height) {
    if (root == NULL) return;
    
    (*size)++;
    *sum += root->key * root->weight;
    
    if (current_height > *height) {
        *height = current_height;
    }
    
    *weighted_height += root->weight * current_height;
    
    calculate_tree_stats(root->left, size, sum, height, weighted_height, current_height + 1);
    calculate_tree_stats(root->right, size, sum, height, weighted_height, current_height + 1);
}

void print_matrix(int matrix[][N+1], int n, const char* name, int display_size) {
    printf("\n%s (первые %dx%d элементов):\n", name, display_size, display_size);
    printf("    ");
    for (int j = 0; j < display_size; j++) {
        printf("%4d ", j);
    }
    printf("\n");
    
    for (int i = 0; i < display_size; i++) {
        printf("%2d: ", i);
        for (int j = 0; j < display_size; j++) {
            if (j >= i) {
                printf("%4d ", matrix[i][j]);
            } else {
                printf("     ");
            }
        }
        printf("\n");
    }
}

void print_compact_matrix(int matrix[][N+1], int n, const char* name) {
    printf("\n%s (компактный вид):\n", name);
    printf("i\\j ");
    for (int j = 0; j <= 10; j++) {
        printf("%4d ", j);
    }
    printf("...\n");
    
    for (int i = 0; i <= 10; i++) {
        printf("%2d: ", i);
        for (int j = 0; j <= 10; j++) {
            if (j >= i) {
                printf("%4d ", matrix[i][j]);
            } else {
                printf("     ");
            }
        }
        printf("\n");
    }
    printf("...\n");
}

int main() {
    int keys[N];
    int weights[N];
    
    // Инициализация случайными весами
    for (int i = 0; i < N; i++) {
        keys[i] = i + 1;
        weights[i] = rand() % 100 + 1;
    }
    
    int AW[N+1][N+1];
    int AP[N+1][N+1];
    int R[N+1][N+1];
    
    Node* root = build_optimal_bst(keys, weights, N, AW, AP, R);
    
    // Вывод матриц
    printf("МАТРИЦЫ ДЛЯ ДЕРЕВА ОПТИМАЛЬНОГО ПОИСКА (n=%d)\n", N);
    printf("===============================================\n");
    
    // Выводим первые 10x10 элементов для наглядности
    int display_size = (N < 10) ? N : 10;
    
    print_matrix(AW, N, "Матрица весов AW", display_size);
    print_matrix(AP, N, "Матрица взвешенных высот AP", display_size);
    print_matrix(R, N, "Матрица корней R", display_size);
    
    // Компактный вид для больших матриц
    if (N > 10) {
        print_compact_matrix(AW, N, "Матрица весов AW");
        print_compact_matrix(AP, N, "Матрица взвешенных высот AP");
        print_compact_matrix(R, N, "Матрица корней R");
    }
    
    // Вывод некоторых ключевых значений
    printf("\nКЛЮЧЕВЫЕ ЗНАЧЕНИЯ:\n");
    printf("AW[0,%d] = %d (сумма всех весов)\n", N, AW[0][N]);
    printf("AP[0,%d] = %d (минимальная взвешенная высота)\n", N, AP[0][N]);
    printf("R[0,%d] = %d (корень всего дерева)\n", N, R[0][N]);
    
    // Обход дерева (выводим только первые 20 элементов для читаемости)
    printf("\nОбход дерева слева направо (первые 20 элементов):\n");
    void print_first_20(Node* root) {
        if (root == NULL) return;
        static int count = 0;
        print_first_20(root->left);
        if (count < 20) {
            printf("%d(%d) ", root->key, root->weight);
            count++;
        }
        print_first_20(root->right);
    }
    print_first_20(root);
    printf("\n");
    
    // Статистика дерева
    int size = 0;
    int control_sum = 0;
    int height = 0;
    double weighted_height = 0.0;
    
    calculate_tree_stats(root, &size, &control_sum, &height, &weighted_height, 1);
    
    if (AW[0][N] > 0) {
        weighted_height /= AW[0][N];
    }
    
    double calculated_weighted_height = (double)AP[0][N] / AW[0][N];
    
    printf("\nСТАТИСТИКА ДЕРЕВА:\n");
   
    printf("│      Параметр   │    Размер  │  Контр.Сумма   │ Высота │ Сред.взвеш.высота  │\n");
    printf("│ ДОП (n=%3d)  │ %10d │ %14d │ %6d │ %19.2f │\n", 
           N, size, control_sum, height, weighted_height);
    
    
    printf("\nПРОВЕРКА ПРАВИЛЬНОСТИ АЛГОРИТМА:\n");
    printf("AP[0,n]/AW[0,n] = %.6f\n", calculated_weighted_height);
    printf("Средневзвешенная высота дерева = %.6f\n", weighted_height);
    printf("Разница = %.10f\n", calculated_weighted_height - weighted_height);
    
    if (fabs(calculated_weighted_height - weighted_height) < 0.0001) {
        printf("✓ Алгоритм работает корректно (разница пренебрежимо мала)\n");
    } 

    
    free_tree(root);
    
    return 0;
}
