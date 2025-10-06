#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

typedef struct {
    int size;
    int sum;
    int height;
    double avg_height;
} TreeStats;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getHeight(AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

AVLNode* createNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rotateLL(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* rotateRR(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

AVLNode* rotateLR(AVLNode* node) {
    node->left = rotateRR(node->left);
    return rotateLL(node);
}

AVLNode* rotateRL(AVLNode* node) {
    node->right = rotateLL(node->right);
    return rotateRR(node);
}

int getBalance(AVLNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* insertAVL(AVLNode* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rotateLL(node);

    if (balance < -1 && key > node->right->key)
        return rotateRR(node);

    if (balance > 1 && key > node->left->key)
        return rotateLR(node);

    if (balance < -1 && key < node->right->key)
        return rotateRL(node);

    return node;
}

void inOrderTraversal(AVLNode* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->key);
        inOrderTraversal(root->right);
    }
}

int countNodes(AVLNode* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void calculateStats(AVLNode* node, int current_height, TreeStats* stats, int* total_height) {
    if (node == NULL) return;

    stats->size++;
    stats->sum += node->key;
    if (current_height > stats->height) {
        stats->height = current_height;
    }
    *total_height += current_height;

    calculateStats(node->left, current_height + 1, stats, total_height);
    calculateStats(node->right, current_height + 1, stats, total_height);
}

AVLNode* createBalancedBST(int arr[], int start, int end) {
    if (start > end) return NULL;

    int mid = (start + end) / 2;
    AVLNode* root = createNode(arr[mid]);

    root->left = createBalancedBST(arr, start, mid - 1);
    root->right = createBalancedBST(arr, mid + 1, end);

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    return root;
}

void freeTree(AVLNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void printStats(const char* name, TreeStats stats) {
    printf("| %-8s | %-8d | %-12d | %-8d | %-12.2f |\n", 
           name, stats.size, stats.sum, stats.height, stats.avg_height);
}

void shuffleArray(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int main() {
    srand(time(NULL));
    
    int numbers[100];
    for (int i = 0; i < 100; i++) {
        numbers[i] = i + 1;
    }
    shuffleArray(numbers, 100);
    
    printf("Случайная последовательность:\n");
    for (int i = 0; i < 100; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");
    
    AVLNode* avlRoot = NULL;
    for (int i = 0; i < 100; i++) {
        avlRoot = insertAVL(avlRoot, numbers[i]);
    }
    
    printf("Обход АВЛ-дерева слева направо:\n");
    inOrderTraversal(avlRoot);
    printf("\n\n");

    for (int i = 0; i < 99; i++) {
        for (int j = 0; j < 99 - i; j++) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
    
    AVLNode* balancedRoot = createBalancedBST(numbers, 0, 99);

    TreeStats avlStats = {0, 0, 0, 0.0};
    TreeStats balancedStats = {0, 0, 0, 0.0};
    
    int avlTotalHeight = 0;
    int balancedTotalHeight = 0;
    
    calculateStats(avlRoot, 1, &avlStats, &avlTotalHeight);
    calculateStats(balancedRoot, 1, &balancedStats, &balancedTotalHeight);
    
    avlStats.avg_height = (double)avlTotalHeight / avlStats.size;
    balancedStats.avg_height = (double)balancedTotalHeight / balancedStats.size;
 
    printf("СРАВНИТЕЛЬНАЯ ТАБЛИЦА ХАРАКТЕРИСТИК:\n");
    printf("| n=100    | Размер  | Контр. Сумма | Высота  | Средн.высота |\n");
    printf("|----------|---------|--------------|---------|--------------|\n");
    printStats("ИСДП", balancedStats);
    printStats("АВЛ", avlStats);

    freeTree(avlRoot);
    freeTree(balancedRoot);

    return 0;
}
