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
    return new_node;
}

void printList(node_t *head){
    node_t *cur = head;
    while (cur){
        printf("%d ",cur->data);
        cur = cur->next;
    }
    printf("\n");
}
node_t* appendBack(node_t* head,int data){
    node_t* new_node = newNode(data);
    
    if (head == NULL) return new_node;

    node_t *cur = head;
    while (cur->next != NULL) cur = cur->next;
    
    cur->next = new_node;
    new_node->prev = cur;
    return head;
}
node_t* createList(){
    node_t* head = NULL;
    while (1){
        int n;
        scanf("%d",&n);
        if(n != 0) head = appendBack(head,n);
        else break;
    }
    return head;
}
void LinkLists(node_t* head1, node_t* head2){
    while (head1 != NULL && head2 != NULL){
        head1->prev = head2;
        head1 = head1->next;
        head2 = head2->next;
    }
    while (head1 != NULL) {
        head1->next = NULL;
        head1 = head1->next;
    }
}
void NaViList(node_t* S){
    node_t* cur = S;
    char input,choice;
    while (1) {
        printf("Текукщий элемент %d\n",cur->data);
        printf("S - вниз D - Вправо 0 - Для выхода\n");
        scanf(" %c", &input);

        if (input == '0') break;
        switch (input) {
            case 'D': 
                if (cur->next) {
                    cur = cur->next;
                } else {
                    printf("Достигнут конец списка. Вернуться в начало? Y/N\n");
                    scanf(" %c", &choice);
                    if (choice == 'Y') cur = S;  
                }
                break;
            case 'S':
                if (cur->prev) cur = cur->prev;
                break;
        }
    }
}
int main() {
    node_t* list_1 = createList();
    node_t* list_2 = createList();
    node_t* head1 = list_1;
    node_t* head2 = list_2;
    LinkLists(head1, head2);

    printList(list_1);
    printList(list_2);

    node_t* S = head1;
    NaViList(S);
}
