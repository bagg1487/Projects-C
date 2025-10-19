#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct BSTNode {
    int key;
    struct BSTNode *left, *right;
} BSTNode;


typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left, *right;
} AVLNode;


BSTNode* createBSTNode(int key) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

BSTNode* insertBST(BSTNode* root, int key) {
    if (root == NULL) return createBSTNode(key);
    if (key < root->key)
        root->left = insertBST(root->left, key);
    else if (key > root->key)
        root->right = insertBST(root->right, key);
    return root;
}


int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* createAVLNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->height = 1;
    node->left = node->right = NULL;
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
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

AVLNode* insertAVL(AVLNode* node, int key) {
    if (node == NULL) return createAVLNode(key);
    
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

void inorder(BSTNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

void inorderAVL(AVLNode* root) {
    if (root != NULL) {
        inorderAVL(root->left);
        printf("%d ", root->key);
        inorderAVL(root->right);
    }
}

int treeSize(BSTNode* root) {
    return root ? 1 + treeSize(root->left) + treeSize(root->right) : 0;
}

int treeSizeAVL(AVLNode* root) {
    return root ? 1 + treeSizeAVL(root->left) + treeSizeAVL(root->right) : 0;
}

int treeSum(BSTNode* root) {
    return root ? root->key + treeSum(root->left) + treeSum(root->right) : 0;
}

int treeSumAVL(AVLNode* root) {
    return root ? root->key + treeSumAVL(root->left) + treeSumAVL(root->right) : 0;
}

int treeHeight(BSTNode* root) {
    return root ? 1 + max(treeHeight(root->left), treeHeight(root->right)) : 0;
}

int treeHeightAVL(AVLNode* root) {
    return root ? root->height : 0;
}

double averageDepth(BSTNode* root) {
    if (root == NULL) return 0;
    
    typedef struct {
        BSTNode* node;
        int level;
    } QueueItem;
    
    QueueItem* queue = (QueueItem*)malloc(100 * sizeof(QueueItem));
    int front = 0, rear = 0;
    int totalDepth = 0, nodeCount = 0;
    
    queue[rear].node = root;
    queue[rear].level = 1;
    rear++;
    
    while (front < rear) {
        BSTNode* current = queue[front].node;
        int level = queue[front].level;
        front++;
        
        totalDepth += level;
        nodeCount++;
        
        if (current->left) {
            queue[rear].node = current->left;
            queue[rear].level = level + 1;
            rear++;
        }
        if (current->right) {
            queue[rear].node = current->right;
            queue[rear].level = level + 1;
            rear++;
        }
    }
    
    free(queue);
    return (double)totalDepth / nodeCount;
}

double averageDepthAVL(AVLNode* root) {
    if (root == NULL) return 0;
    
    typedef struct {
        AVLNode* node;
        int level;
    } QueueItem;
    
    QueueItem* queue = (QueueItem*)malloc(100 * sizeof(QueueItem));
    int front = 0, rear = 0;
    int totalDepth = 0, nodeCount = 0;
    
    queue[rear].node = root;
    queue[rear].level = 1;
    rear++;
    
    while (front < rear) {
        AVLNode* current = queue[front].node;
        int level = queue[front].level;
        front++;
        
        totalDepth += level;
        nodeCount++;
        
        if (current->left) {
            queue[rear].node = current->left;
            queue[rear].level = level + 1;
            rear++;
        }
        if (current->right) {
            queue[rear].node = current->right;
            queue[rear].level = level + 1;
            rear++;
        }
    }
    
    free(queue);
    return (double)totalDepth / nodeCount;
}


int main() {
    srand(time(NULL));
    

    int numbers[100];
    for (int i = 0; i < 100; i++) {
        numbers[i] = i + 1;
    }
    
   
    for (int i = 99; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }
    
    BSTNode* bstRoot = NULL;
    AVLNode* avlRoot = NULL;

    for (int i = 0; i < 100; i++) {
        bstRoot = insertBST(bstRoot, numbers[i]);
        avlRoot = insertAVL(avlRoot, numbers[i]);
    }
    

    printf("Обход ДБП (слева-направо):\n");
    inorder(bstRoot);
    printf("\n\n");
    
 
    printf("Обход АВЛ (слева-направо):\n");
    inorderAVL(avlRoot);
    printf("\n\n");
    
   
    int bstSize = treeSize(bstRoot);
    int avlSize = treeSizeAVL(avlRoot);
    int bstSum = treeSum(bstRoot);
    int avlSum = treeSumAVL(avlRoot);
    int bstHeight = treeHeight(bstRoot);
    int avlHeight = treeHeightAVL(avlRoot);
    double bstAvgDepth = averageDepth(bstRoot);
    double avlAvgDepth = averageDepthAVL(avlRoot);
    
   
    printf("n=100    Размер  Контр.Сумма  Высота  Средн.высота\n");
    printf("АВЛ      %-7d %-12d %-7d %-12.2f\n", avlSize, avlSum, avlHeight, avlAvgDepth);
    printf("ДБП      %-7d %-12d %-7d %-12.2f\n", bstSize, bstSum, bstHeight, bstAvgDepth);
    
    return 0;
}
