#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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


void print_matrix(int matrix[][N+1], int n, const char* name) {
    printf("\n%s:\n", name);
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
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
    int keys[N];
    int weights[N];
    

    for (int i = 0; i < N; i++) {
        keys[i] = i + 1;
        weights[i] = rand() % 100 + 1;
    }
    
   
    int AW[N+1][N+1];  
    int AP[N+1][N+1];  
    int R[N+1][N+1];   
    Node* root = build_optimal_bst(keys, weights, N, AW, AP, R);
    

    // printf("Матрицы для дерева оптимального поиска:\n");
    // print_matrix(AW, 10, "Матрица весов AW");
    // print_matrix(AP, 10, "Матрица взвешенных высот AP");
    // print_matrix(R, 10, "Матрица корней R");
    
  
    printf("\nОбход дерева слева направо (ключ(вес)):\n");
    in_order_traversal(root);
    printf("\n\n");
    
    
    int size = 0;
    int control_sum = 0;
    int height = 0;
    double weighted_height = 0.0;
    
    calculate_tree_stats(root, &size, &control_sum, &height, &weighted_height, 1);
    
    if (AW[0][N] > 0) {
        weighted_height /= AW[0][N]; 
    }
    
    double calculated_weighted_height = (double)AP[0][N] / AW[0][N];
    
    printf("n=%d  Размер  Контр.Сумма  Высота  Средневзвеш.высота\n", N);
    printf("ДОП  %6d  %11d  %6d  %19.2f\n", 
           size, control_sum, height, weighted_height);
    
    printf("\nПроверка правильности алгоритма:\n");
    printf("AP[0,n]/AW[0,n] = %.2f\n", calculated_weighted_height);
    printf("Средневзвешенная высота дерева = %.2f\n", weighted_height);
    printf("Разница = %.6f\n", calculated_weighted_height - weighted_height);
  
    free_tree(root);
    
    return 0;
}
