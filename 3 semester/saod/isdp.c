#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Структура узла дерева
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

// Создание нового узла
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Построение ИСДП из отсортированного массива
Node* buildBalancedBST(int arr[], int start, int end) {
    if (start > end) {
        return NULL;
    }
    
    int mid = (start + end) / 2;
    Node* root = createNode(arr[mid]);
    
    root->left = buildBalancedBST(arr, start, mid - 1);
    root->right = buildBalancedBST(arr, mid + 1, end);
    
    return root;
}

// Обход дерева слева направо (симметричный обход)
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->key);
        inOrderTraversal(root->right);
    }
}

// Вычисление размера дерева (количество узлов)
int getSize(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + getSize(root->left) + getSize(root->right);
}

// Вычисление контрольной суммы (сумма всех ключей)
int getChecksum(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return root->key + getChecksum(root->left) + getChecksum(root->right);
}

// Вычисление высоты дерева
int getHeight(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Вычисление суммы высот всех узлов (для средней высоты)
int getTotalHeight(Node* root, int currentHeight) {
    if (root == NULL) {
        return 0;
    }
    return currentHeight + 
           getTotalHeight(root->left, currentHeight + 1) + 
           getTotalHeight(root->right, currentHeight + 1);
}

// Вычисление средней высоты
double getAverageHeight(Node* root) {
    int size = getSize(root);
    if (size == 0) {
        return 0.0;
    }
    int totalHeight = getTotalHeight(root, 1);
    return (double)totalHeight / size;
}

// Функция для отображения дерева (первые 5 уровней)
void printTreeLevel(Node* root, int level, int currentLevel, int* positions) {
    if (root == NULL || currentLevel > level) {
        return;
    }
    
    if (currentLevel == level) {
        printf("%d", root->key);
        return;
    }
    
    printTreeLevel(root->left, level, currentLevel + 1, positions);
    printf("   ");
    printTreeLevel(root->right, level, currentLevel + 1, positions);
}

// Графический вывод первых 5 уровней
void printTreeGraphically(Node* root) {
    printf("\nГрафическое представление первых 5 уровней:\n");
   
    for (int level = 1; level <= 5; level++) {
        printf("Уровень %d: ", level);
        
        // Массив для отслеживания позиций (не используется в упрощенной версии)
        int positions[100] = {0};
        
        // Рекурсивная функция для печати уровня
        void printLevel(Node* node, int current, int target) {
            if (node == NULL) return;
            if (current == target) {
                printf("%d ", node->key);
            } else {
                printLevel(node->left, current + 1, target);
                printLevel(node->right, current + 1, target);
            }
        }
        
        printLevel(root, 1, level);
        printf("\n");
    }
}

// Основная функция
int main() {
    int n;
    
    // Запрос количества вершин
    printf("Введите количество вершин: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Количество вершин должно быть положительным числом.\n");
        return 1;
    }
    
    // Создание отсортированного массива для удобства проверки
    int* arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;  // Последовательные числа для удобства проверки
    }
    
    // Построение ИСДП
    Node* root = buildBalancedBST(arr, 0, n - 1);
    
    // Вывод обхода дерева слева направо
    printf("\nОбход дерева слева направо:\n");
    inOrderTraversal(root);
    printf("\n\n");
    
    // Вычисление характеристик дерева
    int size = getSize(root);
    int checksum = getChecksum(root);
    int height = getHeight(root);
    double avgHeight = getAverageHeight(root);
    
    printf("Характеристики дерева:\n");
    printf("Размер: %d\n", size);
    printf("Контрольная сумма: %d\n", checksum);
    printf("Высота: %d\n", height);
    printf("Средняя высота: %.2f\n", avgHeight);
    printf("\n");
    
    // Графический вывод первых 5 уровней
    if (n >= 1) {
        printTreeGraphically(root);
    }
    
    // Очистка памяти
    free(arr);
}
