#include <stdio.h>
#include <stdlib.h>
struct node {
    struct node *prev;
    int data;
    struct node *next;
};

typedef struct node node_t;

node_t *newNode(int data){
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->prev = NULL;
    new_node->data = data;
    new_node->next = NULL;
}

void printList(node_t *head){
    node_t *cur = head;
    while (cur){
        printf("%d ",cur->data);
        cur = cur->next;
    }
}
node_t* appendBack(node_t* head,int data){
    node_t* new_node = newNode(data);
    
    if (head == NULL) return new_node;
    node_t *cur = head;
    while (cur->next != NULL) cur = cur->next;
    
    cur->next = new_node;
    return head;
}
int main() {
    node_t* head = NULL;
    while (1){
        int n;
        scanf("%d",&n);
        if(n != 0) head = appendBack(head,n);
        else break;
    }
    printList(head);
}
