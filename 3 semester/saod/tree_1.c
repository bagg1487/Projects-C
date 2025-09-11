#include <stdio.h>
#include <stdlib.h>

typedef struct Vertex {
    int val;
    int num;                // номер вершины
    struct Vertex *left;
    struct Vertex *right;
} Vertex;

Vertex* createVertex(int val) {
    Vertex* v = (Vertex*)malloc(sizeof(Vertex));
    v->val = val;
    v->num = 0;
    v->left = NULL;
    v->right = NULL;
    return v;
}

// --- Обходы ---
void Obhod1(Vertex* root) { // Preorder
    if (!root) return;
    printf("%d(%d) ", root->val, root->num);
    Obhod1(root->left);
    Obhod1(root->right);
}

void Obhod2(Vertex* root) { // Inorder
    if (!root) return;
    Obhod2(root->left);
    printf("%d(%d) ", root->val, root->num);
    Obhod2(root->right);
}

void Obhod3(Vertex* root) { // Postorder
    if (!root) return;
    Obhod3(root->left);
    Obhod3(root->right);
    printf("%d(%d) ", root->val, root->num);
}

// Размер дерева
int size(Vertex* root) {
    if (!root) return 0;
    return 1 + size(root->left) + size(root->right);
}

// Контрольная сумма
int checksum(Vertex* root) {
    if (!root) return 0;
    return root->val + checksum(root->left) + checksum(root->right);
}

// Высота дерева
int height(Vertex* root) {
    if (!root) return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return 1 + (hl > hr ? hl : hr);
}

// Сумма глубин
int depthSum(Vertex* root, int d) {
    if (!root) return 0;
    return d + depthSum(root->left, d + 1) + depthSum(root->right, d + 1);
}

// Нумерация вершин по возрастанию (Inorder)
void assignNumbers(Vertex* root, int* counter) {
    if (!root) return;
    assignNumbers(root->left, counter);
    root->num = ++(*counter);
    assignNumbers(root->right, counter);
}

// --- Печать дерева строго как на рисунке (корень сверху) ---
void printTree(Vertex* root, int space) {
    if (root == NULL) return;

    // вывод текущего узла
    for (int i = 0; i < space; i++) putchar(' ');
    printf("%d(%d)\n", root->val, root->num);

    // левое и правое поддерево ниже
    printTree(root->left, space + 5);
    printTree(root->right, space + 5);
}

int main() {
    // ---- Построение дерева (как на картинке) ----
    Vertex* E = createVertex(19);
    Vertex* C = createVertex(15); C->right = E;
    Vertex* B = createVertex(3);  B->right = C;
    Vertex* D = createVertex(5);
    Vertex* A = createVertex(12); A->left = B; A->right = D;
    Vertex* R = createVertex(8);  R->left = A;   // корень

    // ---- Присвоение номеров ----
    int counter = 0;
    assignNumbers(R, &counter);

    // ---- Печать структуры ----
    printf("Структура дерева (корень сверху):\n");
    printTree(R, 0);

    // ---- Обходы ----
    printf("\nObhod1 (сверху вниз): ");
    Obhod1(R); printf("\n");

    printf("Obhod2 (слева направо): ");
    Obhod2(R); printf("\n");

    printf("Obhod3 (снизу вверх): ");
    Obhod3(R); printf("\n");

    // ---- Метрики ----
    int n = size(R);
    int chk = checksum(R);
    int h = height(R);
    double avgDepth = (double)depthSum(R, 0) / n;

    printf("\nРазмер дерева: %d\n", n);
    printf("Контрольная сумма: %d\n", chk);
    printf("Высота: %d\n", h);
    printf("Средняя глубина: %.2f\n", avgDepth);

    return 0;
}
