#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

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

int getBalance(AVLNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
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


AVLNode* rotateLL1(AVLNode* y, int* heightDecreased) {
    AVLNode* x = y->left;
    
    if (getBalance(x) == 0) {
        *heightDecreased = 0; // Умен = нет
    } else {
        *heightDecreased = 1;
    }

    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

AVLNode* rotateRR1(AVLNode* x, int* heightDecreased) {
    AVLNode* y = x->right;
    
    if (getBalance(y) == 0) {
        *heightDecreased = 0; // Умен = нет
    } else {
        *heightDecreased = 1;
    }

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

AVLNode* findMin(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}


AVLNode* deleteAVL(AVLNode* root, int key, int* heightDecreased) {
    if (root == NULL) {
        *heightDecreased = 0;
        return root;
    }

    if (key < root->key) {
        root->left = deleteAVL(root->left, key, heightDecreased);
        if (*heightDecreased) {
            root->height = 1 + max(getHeight(root->left), getHeight(root->right));
            int balance = getBalance(root);
            
            if (balance > 1) {
                if (getBalance(root->left) >= 0) {
                    return rotateLL1(root, heightDecreased);
                } else {
                    return rotateLR(root);
                }
            } else if (balance < -1) {
                *heightDecreased = 0;
            }
        }
    } else if (key > root->key) {
        root->right = deleteAVL(root->right, key, heightDecreased);
        if (*heightDecreased) {
            root->height = 1 + max(getHeight(root->left), getHeight(root->right));
            int balance = getBalance(root);
            
            if (balance < -1) {
                if (getBalance(root->right) <= 0) {
                    return rotateRR1(root, heightDecreased);
                } else {
                    return rotateRL(root);
                }
            } else if (balance > 1) {
                *heightDecreased = 0;
            }
        }
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            *heightDecreased = 1;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            AVLNode* temp = findMin(root->right);
            root->key = temp->key;
            root->right = deleteAVL(root->right, temp->key, heightDecreased);
            if (*heightDecreased) {
                root->height = 1 + max(getHeight(root->left), getHeight(root->right));
                int balance = getBalance(root);
                
                if (balance > 1) {
                    if (getBalance(root->left) >= 0) {
                        return rotateLL1(root, heightDecreased);
                    } else {
                        return rotateLR(root);
                    }
                }
            }
        }
    }

    if (root == NULL) return root;

    return root;
}

void inOrderTraversal(AVLNode* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->key);
        inOrderTraversal(root->right);
    }
}

void freeTree(AVLNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    srand(time(NULL));
    
    AVLNode* root = NULL;
    for (int i = 1; i <= 100; i++) {
        root = insertAVL(root, i);
    }
    
    printf("Исходное АВЛ-дерево (обход слева направо):\n");
    inOrderTraversal(root);
    printf("\n\n");
    
    printf("УДАЛЕНИЕ 10 ВЕРШИН ИЗ АВЛ-ДЕРЕВА:\n");
    printf("Введите 10 чисел для удаления: ");
    
    for (int i = 0; i < 10; i++) {
        int key;
        scanf("%d", &key);
        
        printf("\n Удаление вершины %d \n", key);
        int heightDecreased = 0;
        root = deleteAVL(root, key, &heightDecreased);
        
        printf("Обход дерева слева направо после удаления:\n");
        inOrderTraversal(root);
        printf("\n");
    }
    freeTree(root);
    
}
