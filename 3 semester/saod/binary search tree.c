#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left, *right;
} AVLNode;

typedef struct DBDNode {
    int key;
    int bal;
    struct DBDNode *left, *right;
} DBDNode;

int VR = 1;
int HR = 1;

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

DBDNode* createDBDNode(int key) {
    DBDNode* node = (DBDNode*)malloc(sizeof(DBDNode));
    node->key = key;
    node->left = node->right = NULL;
    node->bal = 0;
    return node;
}

DBDNode* B2INSERT(int D, DBDNode *p) {
    if (p == NULL) {
        p = createDBDNode(D);
        VR = 1;
        return p;
    }
    
    if (p->key > D) {
        p->left = B2INSERT(D, p->left);
        if (VR == 1) {
            if (p->bal == 0) {
                DBDNode* q = p->left;
                p->left = q->right;
                q->right = p;
                p = q;
                p->bal = 1;
                VR = 0;
                HR = 1;
            } else {
                p->bal = 0;
                VR = 1;
                HR = 0;
            }
        } else {
            HR = 0;
        }
    } else if (p->key < D) {
        p->right = B2INSERT(D, p->right);
        if (VR == 1) {
            p->bal = 1;
            HR = 1;
            VR = 0;
        } else if (HR == 1) {
            if (p->bal == 1) {
                DBDNode* q = p->right;
                p->bal = 0;
                q->bal = 0;
                p->right = q->left;
                q->left = p;
                p = q;
                VR = 1;
                HR = 0;
            } else {
                HR = 0;
            }
        }
    }
    return p;
}

void inorderAVL(AVLNode* root) {
    if (root != NULL) {
        inorderAVL(root->left);
        printf("%d ", root->key);
        inorderAVL(root->right);
    }
}

void inorderDBD(DBDNode* root) {
    if (root != NULL) {
        inorderDBD(root->left);
        printf("%d ", root->key);
        inorderDBD(root->right);
    }
}

int treeSizeAVL(AVLNode* root) {
    return root ? 1 + treeSizeAVL(root->left) + treeSizeAVL(root->right) : 0;
}

int treeSizeDBD(DBDNode* root) {
    return root ? 1 + treeSizeDBD(root->left) + treeSizeDBD(root->right) : 0;
}

int treeSumAVL(AVLNode* root) {
    return root ? root->key + treeSumAVL(root->left) + treeSumAVL(root->right) : 0;
}

int treeSumDBD(DBDNode* root) {
    return root ? root->key + treeSumDBD(root->left) + treeSumDBD(root->right) : 0;
}

int treeHeightAVL(AVLNode* root) {
    return root ? root->height : 0;
}

int treeHeightDBD(DBDNode* root) {
    return root ? 1 + max(treeHeightDBD(root->left), treeHeightDBD(root->right)) : 0;
}

double averageDepthAVL(AVLNode* root) {
    if (root == NULL) return 0;
    
    typedef struct {
        AVLNode* node;
        int level;
    } QueueItem;
    
    QueueItem* queue = (QueueItem*)malloc(1000 * sizeof(QueueItem));
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

double averageDepthDBD(DBDNode* root) {
    if (root == NULL) return 0;
    
    typedef struct {
        DBDNode* node;
        int level;
    } QueueItem;
    
    QueueItem* queue = (QueueItem*)malloc(1000 * sizeof(QueueItem));
    int front = 0, rear = 0;
    int totalDepth = 0, nodeCount = 0;
    
    queue[rear].node = root;
    queue[rear].level = 1;
    rear++;
    
    while (front < rear) {
        DBDNode* current = queue[front].node;
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
    
    AVLNode* avlRoot = NULL;
    DBDNode* dbdRoot = NULL;

    for (int i = 0; i < 100; i++) {
        avlRoot = insertAVL(avlRoot, numbers[i]);
        VR = 1;
        HR = 1;
        dbdRoot = B2INSERT(numbers[i], dbdRoot);
    }
    
    printf("Обход АВЛ:\n");
    inorderAVL(avlRoot);
    printf("\n\n");
    
    printf("Обход ДБД:\n");
    inorderDBD(dbdRoot);
    printf("\n\n");
    
    int avlSize = treeSizeAVL(avlRoot);
    int dbdSize = treeSizeDBD(dbdRoot);
    int avlSum = treeSumAVL(avlRoot);
    int dbdSum = treeSumDBD(dbdRoot);
    int avlHeight = treeHeightAVL(avlRoot);
    int dbdHeight = treeHeightDBD(dbdRoot);
    double avlAvgDepth = averageDepthAVL(avlRoot);
    double dbdAvgDepth = averageDepthDBD(dbdRoot);
    
    printf("n=100    Размер  Контр.Сумма  Высота  Средн.высота\n");
    printf("АВЛ      %-7d %-12d %-7d %-12.2f\n", avlSize, avlSum, avlHeight, avlAvgDepth);
    printf("ДБД      %-7d %-12d %-7d %-12.2f\n", dbdSize, dbdSum, dbdHeight, dbdAvgDepth);
    
    return 0;
}
