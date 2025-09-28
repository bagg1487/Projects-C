#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

// Рекурсивная вставка в СДП
Node* insertRecursive(Node* root, int key) {
    if (root == NULL) {
        return createNode(key);
    }
    
    if (key < root->key) {
        root->left = insertRecursive(root->left, key);
    } else if (key > root->key) {
        root->right = insertRecursive(root->right, key);
    }
    
    return root;
}

// Вставка с двойной косвенностью
void insertDoubleIndirect(Node** root, int key) {
    Node** current = root;
    
    while (*current != NULL) {
        if (key < (*current)->key) {
            current = &(*current)->left;
        } else if (key > (*current)->key) {
            current = &(*current)->right;
        } else {
            return; 
        }
    }
    
    *current = createNode(key);
}

// Поиск минимального узла в дереве
Node* findMin(Node* root) {
    while (root != NULL && root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Функция удаления вершины с заданным ключом (рекурсивная)
Node* deleteNode(Node* root, int key) {
    if (root == NULL) {
        return NULL;
    }
    
    // Поиск удаляемой вершины
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        // Нашли вершину для удаления
        
        // Случай 1: Нет детей или один ребенок
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        
        // Случай 2: Два ребенка
        // Находим минимальный узел в правом поддереве
        Node* temp = findMin(root->right);
        
        // Копируем данные минимального узла
        root->key = temp->key;
        
        // Рекурсивно удаляем минимальный узел из правого поддерева
        root->right = deleteNode(root->right, temp->key);
    }
    
    return root;
}

// Удаление с двойной косвенностью
void deleteDoubleIndirect(Node** root, int key) {
    Node** current = root;
    
    // Поиск вершины для удаления
    while (*current != NULL) {
        if (key < (*current)->key) {
            current = &(*current)->left;
        } else if (key > (*current)->key) {
            current = &(*current)->right;
        } else {
            // Нашли вершину для удаления
            Node* toDelete = *current;
            
            // Случай 1: Нет детей или один ребенок
            if (toDelete->left == NULL) {
                *current = toDelete->right;
                free(toDelete);
            } else if (toDelete->right == NULL) {
                *current = toDelete->left;
                free(toDelete);
            } else {
                // Случай 2: Два ребенка
                // Находим минимальный узел в правом поддереве
                Node* minNode = findMin(toDelete->right);
                
                // Копируем данные минимального узла
                toDelete->key = minNode->key;
                
                // Рекурсивно удаляем минимальный узел из правого поддерева
                deleteDoubleIndirect(&toDelete->right, minNode->key);
            }
            break;
        }
    }
}

// Обход слева направо (симметричный)
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->key);
        inOrderTraversal(root->right);
    }
}

// Вычисление характеристик дерева
int getSize(Node* root) {
    if (root == NULL) return 0;
    return 1 + getSize(root->left) + getSize(root->right);
}

int getChecksum(Node* root) {
    if (root == NULL) return 0;
    return root->key + getChecksum(root->left) + getChecksum(root->right);
}

int getHeight(Node* root) {
    if (root == NULL) return 0;
    int left = getHeight(root->left);
    int right = getHeight(root->right);
    return 1 + (left > right ? left : right);
}

int getTotalHeight(Node* root, int depth) {
    if (root == NULL) return 0;
    return depth + getTotalHeight(root->left, depth + 1) + getTotalHeight(root->right, depth + 1);
}

double getAverageHeight(Node* root) {
    int size = getSize(root);
    if (size == 0) return 0.0;
    return (double)getTotalHeight(root, 1) / size;
}

// Генерация случайной последовательности
void generateRandomSequence(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    
    // Перемешиваем Фишером-Йейтсом
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Функция для красивого вывода дерева (вертикальный обход)
void printTree(Node* root, int space) {
    if (root == NULL) return;
    
    // Увеличиваем отступ
    space += 5;
    
    // Сначала правый ребенок
    printTree(root->right, space);
    
    // Текущий узел
    printf("\n");
    for (int i = 5; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->key);
    
    // Левый ребенок
    printTree(root->left, space);
}

// Основная функция
int main() {
    srand(time(NULL));
    const int n = 100;
    int sequence[n];
    
    // Генерируем случайную последовательность
    generateRandomSequence(sequence, n);
    
    // Строим два СДП
    Node* sdp1Root = NULL; // СДП рекурсивное
    Node* sdp2Root = NULL; // СДП с двойной косвенностью
    
    // Заполняем СДП
    for (int i = 0; i < n; i++) {
        sdp1Root = insertRecursive(sdp1Root, sequence[i]);
        insertDoubleIndirect(&sdp2Root, sequence[i]);
    }
    
    // Выводим обходы
    printf("Обход СДП1 (рекурсивное): ");
    inOrderTraversal(sdp1Root);
    printf("\n\n");
    
    printf("Обход СДП2 (двойная косвенность): ");
    inOrderTraversal(sdp2Root);
    printf("\n\n");
    
    // Вычисляем характеристики
    printf("| n=%d | Размер | Контр.сумма | Высота | Средн.высота |\n", n);
    printf("|-------|--------|-------------|--------|--------------|\n");
    
    printf("| СДП1  | %6d | %11d | %6d | %12.2f |\n", 
           getSize(sdp1Root), getChecksum(sdp1Root), 
           getHeight(sdp1Root), getAverageHeight(sdp1Root));
    
    printf("| СДП2  | %6d | %11d | %6d | %12.2f |\n", 
           getSize(sdp2Root), getChecksum(sdp2Root), 
           getHeight(sdp2Root), getAverageHeight(sdp2Root));
    
    // Удаление 10 вершин из первого СДП
    printf("\n=== УДАЛЕНИЕ 10 ВЕРШИН ИЗ СДП1 (рекурсивное) ===\n");
    
    for (int i = 0; i < 10; i++) {
        int keyToDelete;
        printf("\nВведите ключ для удаления %d: ", i + 1);
        scanf("%d", &keyToDelete);
        
        printf("Удаляем вершину с ключом %d...\n", keyToDelete);
        
        // Удаляем из первого СДП
        sdp1Root = deleteNode(sdp1Root, keyToDelete);
        
        // Выводим обход после удаления
        printf("Обход после удаления: ");
        inOrderTraversal(sdp1Root);
        printf("\n");
        
        // Выводим характеристики
        printf("Размер: %d, Контр.сумма: %d, Высота: %d, Средняя высота: %.2f\n",
               getSize(sdp1Root), getChecksum(sdp1Root), getHeight(sdp1Root), getAverageHeight(sdp1Root));
        
        printf("------------------------\n");
    }
    
    // Удаление 10 вершин из второго СДП
    printf("\nУДАЛЕНИЕ 10 ВЕРШИН ИЗ СДП\n");
    
    for (int i = 0; i < 10; i++) {
        int keyToDelete;
        printf("\nВведите ключ для удаления %d: ", i + 1);
        scanf("%d", &keyToDelete);
        
        printf("Удаляем вершину с ключом %d...\n", keyToDelete);
        
        // Удаляем из второго СДП
        deleteDoubleIndirect(&sdp2Root, keyToDelete);
        
        // Выводим обход после удаления
        printf("Обход после удаления: ");
        inOrderTraversal(sdp2Root);
        printf("\n");
        
        // Выводим характеристики
        printf("Размер: %d, Контр.сумма: %d, Высота: %d, Средняя высота: %.2f\n",
               getSize(sdp2Root), getChecksum(sdp2Root), getHeight(sdp2Root), getAverageHeight(sdp2Root));
        
        printf("------------------------\n");
    }
    
    return 0;
}
