#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/tree.h"

int get_height(TreeNode* node) {
    return node ? node->height : 0;
}

int max_int(int a, int b) {
    return (a > b) ? a : b;
}

TreeNode* create_tree_node(Record* record) {
    TreeNode* node = malloc(sizeof(TreeNode));
    strcpy(node->key, record->fio);
    node->data = record;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

TreeNode* rotate_right(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = max_int(get_height(y->left), get_height(y->right)) + 1;
    x->height = max_int(get_height(x->left), get_height(x->right)) + 1;
    
    return x;
}

TreeNode* rotate_left(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = max_int(get_height(x->left), get_height(x->right)) + 1;
    y->height = max_int(get_height(y->left), get_height(y->right)) + 1;
    
    return y;
}

int get_balance(TreeNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

TreeNode* insert_tree_node(TreeNode* node, Record* record) {
    if (!node) return create_tree_node(record);
    
    int cmp = strcmp(record->fio, node->key);
    
    if (cmp < 0) node->left = insert_tree_node(node->left, record);
    else if (cmp > 0) node->right = insert_tree_node(node->right, record);
    else return node;
    
    node->height = 1 + max_int(get_height(node->left), get_height(node->right));
    
    int balance = get_balance(node);
    
    if (balance > 1 && strcmp(record->fio, node->left->key) < 0) return rotate_right(node);
    if (balance < -1 && strcmp(record->fio, node->right->key) > 0) return rotate_left(node);
    if (balance > 1 && strcmp(record->fio, node->left->key) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && strcmp(record->fio, node->right->key) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    
    return node;
}

TreeNode* create_tree_from_queue(QueueNode* queue) {
    TreeNode* root = NULL;
    QueueNode* current = queue;
    
    while (current) {
        root = insert_tree_node(root, current->data);
        current = current->next;
    }
    
    return root;
}

void search_tree_by_fio_prefix_helper(TreeNode* root, const char* fio_prefix, QueueNode** head, QueueNode** tail) {
    if (!root) return;
    
    search_tree_by_fio_prefix_helper(root->left, fio_prefix, head, tail);
    
    if (strncmp(root->key, fio_prefix, 3) == 0) {
        QueueNode* new_node = malloc(sizeof(QueueNode));
        new_node->data = root->data;
        new_node->next = NULL;
        
        if (!*head) *head = new_node;
        else (*tail)->next = new_node;
        *tail = new_node;
    }
    
    search_tree_by_fio_prefix_helper(root->right, fio_prefix, head, tail);
}

QueueNode* search_tree_by_fio_prefix(TreeNode* root, const char* fio_prefix) {
    QueueNode* head = NULL;
    QueueNode* tail = NULL;
    
    search_tree_by_fio_prefix_helper(root, fio_prefix, &head, &tail);
    return head;
}

void print_tree_inorder(TreeNode* root, int* counter) {
    if (!root) return;
    
    print_tree_inorder(root->left, counter);
    
    printf("│ %-3d │ %-30s │ %-16s │ %-4d │ %-7d │ %-10s │\n",
           (*counter)++, root->data->fio, root->data->street,
           root->data->house, root->data->apartment, root->data->date);
    
    print_tree_inorder(root->right, counter);
}

void print_tree_table(TreeNode* root) {
    if (!root) {
        printf("Дерево пустое\n");
        return;
    }
    
    printf("\nСодержимое АВЛ-дерева (по ФИО):\n");
    printf("┌─────┬────────────────────────────────┬──────────────────┬──────┬─────────┬────────────┐\n");
    printf("│ №   │ ФИО                            │ Улица            │ Дом  │Квартира │ Дата       │\n");
    printf("├─────┼────────────────────────────────┼──────────────────┼──────┼─────────┼────────────┤\n");
    
    int counter = 1;
    print_tree_inorder(root, &counter);
    
    printf("└─────┴────────────────────────────────┴──────────────────┴──────┴─────────┴────────────┘\n");
}

void free_tree(TreeNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}