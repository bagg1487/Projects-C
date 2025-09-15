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

// Построение полного бинарного дерева с нумерацией сверху-вниз слева-направо
Node* buildLevelOrderTree(int n) {
    if (n <= 0) {
        return NULL;
    }
    
    // Создаем массив узлов
    Node** nodes = (Node**)malloc(n * sizeof(Node*));
    for (int i = 0; i < n; i++) {
        nodes[i] = createNode(i + 1);
    }
    
    // Связываем узлы в дерево
    for (int i = 0; i < n; i++) {
        int leftIndex = 2 * i + 1;
        int rightIndex = 2 * i + 2;
        
        if (leftIndex < n) {
            nodes[i]->left = nodes[leftIndex];
        }
        if (rightIndex < n) {
            nodes[i]->right = nodes[rightIndex];
        }
    }
    
    Node* root = nodes[0];
    free(nodes);
    return root;
}

// Обход дерева слева-направо (симметричный обход)
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->key);
        inOrderTraversal(root->right);
    }
}
void preOrderTraversal(Node* root) {
    if (root != NULL) {
        printf("%d ", root->key);  // Сначала корень
        preOrderTraversal(root->left);  // Затем левое поддерево
        preOrderTraversal(root->right);  // Затем правое поддерево
    }
}
// Вычисление размера дерева
int getSize(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + getSize(root->left) + getSize(root->right);
}

// Вычисление контрольной суммы
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

// Вычисление суммы высот всех узлов
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

// Печать уровня дерева
void printLevel(Node* root, int level) {
    if (root == NULL) {
        printf("  ");
        return;
    }
    if (level == 1) {
        printf("%2d ", root->key);
    } else if (level > 1) {
        printLevel(root->left, level - 1);
        printLevel(root->right, level - 1);
    }
}


// Основная функция
int main() {
    int n;
    
    printf("Введите количество вершин: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Количество вершин должно быть положительным числом.\n");
        return 1;
    }
    
    // Построение дерева с нумерацией сверху-вниз слева-направо
    Node* root = buildLevelOrderTree(n);
    
    \
    
    // Вывод обхода слева-направо (симметричный)
    printf("Обход слева-направо: ");
    inOrderTraversal(root);
    printf("\n\n");
    
    // printf("Обход сверху-вниз: ");
    // preOrderTraversal(root);
    // printf("\n\n");
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
}
