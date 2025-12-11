#ifndef TREE_H
#define TREE_H

#include "database.h"
#include "search.h"

typedef struct TreeNode {
    char key[FIO_SIZE + 1];
    Record* data;
    struct TreeNode* left;
    struct TreeNode* right;
    int height;
} TreeNode;

TreeNode* create_tree_from_queue(QueueNode* queue);
void print_tree_table(TreeNode* root);
QueueNode* search_tree_by_fio_prefix(TreeNode* root, const char* fio_prefix);
TreeNode* search_tree_by_field(TreeNode* root, const char* field_name, const char* value);
void free_tree(TreeNode* root);

#endif