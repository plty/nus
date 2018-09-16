/*************************************
 * Lab 1 Exercise 1
 * Name: Jerrell Ezralemuel
 * Matric No: A0181002B
 * Lab Group: 08
 *************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

node* addToHead(node*, int);
void printList(node* );
void destroyList(node* );

int main() {
    node* myList = NULL;
    int input;

    while (scanf("%i",&input) != EOF){
        myList = addToHead(myList, input);
    }

    //Print out the linked list
    printf("My List:\n");
    printList(myList);

    destroyList(myList);
    myList = NULL;

    //Print out the linked list after destruction
    printf("My List After Destroy:\n");
    printList(myList);

    return 0;
}

node* addToHead(node* head, int newData) {
    node* n = (node*) malloc(sizeof(node));
    n->next = head;
    n->data = newData;
    return n;
}

void printList(node* head) {
    node* ptr = head;
    while (ptr)  {
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node* head) {
    if (head == NULL) return;
    node *prev = head;
    for (node *cur = head->next; cur != NULL; prev = cur, cur = cur->next)
        free(prev);
    free(prev);
}
